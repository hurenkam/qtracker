#include <QtXml/QDomDocument>
#include <QMetaMethod>
#include <QTimerEvent>
#include <QStringList>
#include <QDateTime>
#include <QtNetwork/QSslSocket>
#include <QFile>
#include "xmlrpcserver.h"
#include "xmlrpcconv.h"

#ifndef QT_NO_OPENSSL
SslParams::SslParams ( const QString &certFile, const QString &keyFile, QObject *parent ) :
    QObject( parent )
{
    QFile   fc( certFile, this );
    fc.open( QFile::ReadOnly );
    certificate= QSslCertificate( fc.readAll() );
    fc.close();
    ca << certificate;
    QFile   fk( keyFile, this );
    fk.open( QFile::ReadOnly );
    privateKey= QSslKey( fk.readAll(), QSsl::Rsa, QSsl::Pem, QSsl::PrivateKey );
    fk.close();
}
#endif

Protocol::Protocol( QAbstractSocket *parent, const int _timeout ) :
    QObject( parent ),
    timeout( 0 ),
    timeoutTimerId( 0 ),
    socket( parent )
{
    #ifdef DEBUG_PROTOCOL
    qDebug() << this << "Protocol(): child of" << socket;
    #endif

    /* check we are only one protocol, working with this socket, at this time */
    foreach( QObject * o, parent->children() ) if ( o != this && qobject_cast < Protocol * > (o) )
      {
        qCritical() << this << "Protocol(): socket" << parent << "already have another protocol" << o;
        qFatal( "programming error" );
      }

    if ( _timeout > 0 ) setTimeout( _timeout );
}

Protocol::~Protocol()
{
    #ifdef DEBUG_PROTOCOL
    qDebug() << this << "~Protocol()";
    #endif
}

/*
 =======================================================================================================================
    timeout restarts
 =======================================================================================================================
 */
void Protocol::setTimeout( const int _timeout )
{
    #ifdef DEBUG_PROTOCOL
    qDebug() << this << "setTimeout():" << _timeout << "prev timeout was" << timeout;
    #endif
    if ( timeoutTimerId )
      {
        #ifdef DEBUG_PROTOCOL
        qDebug() << this << "setTimeout(): stopping previous timer";
        #endif
        killTimer( timeoutTimerId );
        timeoutTimerId= 0;
      }

    if ( !(timeout > 0) && _timeout > 0 )
      {

        /*
         * если предыдущий timeout НЕ был больше ноля, а текущий больше,
         * значит нужно прицепить сигналы приема/отсылки данных на сокете
         */
        #ifdef DEBUG_PROTOCOL
        qDebug() << this << "setTimeout(): connect socket signals readyRead() and bytesWritten()";
        #endif
        connect( socket, SIGNAL( readyRead()), this, SLOT( __slotReadyRead()) );
        connect( socket, SIGNAL( bytesWritten( qint64)), this, SLOT( __slotBytesWritten( qint64)) );
      }
    else if ( timeout > 0 && !(_timeout > 0) )
      {

        /* новый выключен, старый был включен */
        #ifdef DEBUG_PROTOCOL
        qDebug() << this << "setTimeout(): disconnect socket signals readyRead() and bytesWritten()";
        #endif
        disconnect( socket, SIGNAL( readyRead()), this, SLOT( __slotReadyRead()) );
        disconnect( socket, SIGNAL( bytesWritten( qint64)), this, SLOT( __slotBytesWritten( qint64)) );
      }

    timeout= _timeout;
    if ( timeout > 0 ) restartProtocolTimeoutTimer();
}

void Protocol::restartProtocolTimeoutTimer()
{
    #ifdef DEBUG_PROTOCOL
    qDebug() << this << "restartProtocolTimeoutTimer()";
    #endif
    if ( timeoutTimerId ) killTimer( timeoutTimerId );
    Q_ASSERT( timeout > 0 );
    timeoutTimerId= startTimer( timeout );
}

void Protocol::timerEvent( QTimerEvent *event )
{
    #ifdef DEBUG_PROTOCOL
    qDebug() << this << "timerEvent():" << event->timerId() << "protocol timeout timer id" << timeoutTimerId;
    #endif
    if ( event->timerId() == timeoutTimerId )
      {
        #ifdef DEBUG_PROTOCOL
        qDebug() << this << "timerEvent(): emit ProtocolTimeout()";
        #endif

        emit    protocolTimeout( this );
        killTimer( timeoutTimerId );
        timeoutTimerId= 0;
      }
}

void Protocol::__slotReadyRead()
{
    #ifdef DEBUG_PROTOCOL
    qDebug() << this << "__slotReadyRead():" << socket->bytesAvailable()
        << "bytes available, restarting protocol timeout timer";
    #endif
    restartProtocolTimeoutTimer();
}

void Protocol::__slotBytesWritten( qint64  bytes  )
{
    #ifdef DEBUG_PROTOCOL
    qDebug() << this << "__slotBytesWritten():" << bytes << "written, restarting protocol timeout timer";
    #else
    Q_UNUSED(bytes)
    #endif
    restartProtocolTimeoutTimer();
}

/*
 =======================================================================================================================
    static QHttpResponseHeader xmlRpcResponseHeader( const qint64 contentLength ) { #ifdef DEBUG_XMLRPC qDebug() <<
    "xmlRpcHeader():" << contentLength;
    #endif QHttpResponseHeader h( 200, "OK", 1, 0 );
    h.setContentType( "text/xml" );
    h.setContentLength( contentLength );
    h.setValue( "connection", "close" );
    h.setValue( "server", "qt-xmlrpc" );
    return h;
    }
 =======================================================================================================================
 */
HttpServer::HttpServer(QAbstractSocket *parent , const int _timeout) :
    Protocol( parent, _timeout ),
    state( ReadingHeader )
{
    #ifdef DEBUG_XMLRPC
    qDebug() << this << "HttpServer():" << parent;
    #endif
    connect( socket, SIGNAL( readyRead()), this, SLOT( slotReadyRead()) );
    connect( socket, SIGNAL( bytesWritten( qint64)), this, SLOT( slotBytesWritten( qint64)) );
    #ifndef QT_NO_OPENSSL
    if ( socket->inherits( "QSslSocket") )
        {
            QSslSocket  *sslServer= qobject_cast < QSslSocket * > ( socket );
            sslServer->startServerEncryption();
        }
    else
        {
            if ( socket->bytesAvailable() > 0 )
                {
                    slotReadyRead();
                }
        }

    #else
    if ( socket->bytesAvailable() > 0 )
      {
        slotReadyRead();
      }
    #endif
}

void HttpServer::slotReadyRead()
{
    #ifdef DEBUG_XMLRPC
    qDebug() << this << "slotReadyRead():" << socket->bytesAvailable() << "bytes available";
    #endif
    if ( !socket->bytesAvailable() ) return;
    switch ( state )
        {
        case ReadingHeader:

            /* если заголовок прочитан */
            if ( readRequestHeader() )
              {

                /*
                 * если судя по заголовку есть тело, меняем
                 * статус на чтение тела ;
                 * и попадаем в следующий case
                 */
                if ( requestContainsBody() ) state= ReadingBody;
              } else
              {

                /*
                 * тела нет, бросаем сигнал, что заголовок
                 * получен
                 */
                Q_ASSERT( !socket->bytesAvailable() );
                state= WaitingReply;
                #ifdef DEBUG_XMLRPC
                qDebug() << this << "slotReadyRead(): emit requestReceived()";
                #endif

                emit    requestReceived( this, requestHeader, requestBody );
                break;
              }

        case ReadingBody:
            if ( readRequestBody() )
              {

                /*
                 * тело прочитано, бросаем сигнал, что запрос получен
                 */
                Q_ASSERT( !socket->bytesAvailable() );
                state= WaitingReply;
                #ifdef DEBUG_XMLRPC
                qDebug() << this << "slotReadyRead(): emit requestReceived()";
                #endif

                emit    requestReceived( this, requestHeader, requestBody );
              }
            break;
        case WaitingReply:
            qCritical() << this << "slotReadyRead(): got data in WaitingReply state, emit parseError()";

            emit    parseError( this );
            break;
        case SendingReply:
            qCritical() << this << "slotReadyRead(): got data in SendingHeader state, emit parseError()";

            emit    parseError( this );
            break;
        case Done:
            qCritical() << this << "slotReadyRead(): got data in Done state, emit parseError()";

            emit    parseError( this );
            break;
        default:
            qCritical() << this << "slotReadyRead(): unknown state";
            qFatal( "programming error" );
        }
}

bool HttpServer::readRequestHeader()
{
    #ifdef DEBUG_XMLRPC
    qDebug() << this << "readRequestHeader()";
    #endif

    /* code from qhttp.cpp */
    bool        end = false;
    QByteArray  tmp;
    QByteArray rn( "\r\n", 2 ), n( "\n", 1 );
    while ( !end && socket->canReadLine() )
      {
        tmp = socket->readLine();
        if ( tmp == rn || tmp == n || tmp.isEmpty() )
            end = true;
        else
            requestHeaderBody.append( tmp );
      }

    if ( !end )
      {
        #ifdef DEBUG_XMLRPC
        qDebug() << this << "readRequestHeader(): waiting more data, readed" << endl << requestHeaderBody;
        #endif
        return false;
      }

    requestHeader= QHttpRequestHeader( requestHeaderBody );
    requestHeaderBody.clear();
    requestBody.clear();
    if ( requestHeader.isValid() )
      {
        #ifdef DEBUG_XMLRPC
        qDebug() << this << "readRequestHeader(): header valid" << endl << requestHeader.toString();
        #endif
        return true;
      }

    qWarning() << this << "readRequestHeader(): invalid requestHeader, emit parseError()"
               << endl << requestHeader.toString();

    emit    parseError( this );
    return false;
}

bool HttpServer::readRequestBody()
{
    Q_ASSERT( requestHeader.isValid() );

    qint64  bytesToRead= ( qint64 ) requestHeader.contentLength() - ( qint64 ) requestBody.size();
    if ( bytesToRead > socket->bytesAvailable() ) bytesToRead= socket->bytesAvailable();
    #ifdef DEBUG_XMLRPC
    qDebug() << this << "readRequestBody(): already read" << requestBody.size() << "to read" << bytesToRead;
    #endif
    requestBody.append( socket->read( bytesToRead) );
    #ifdef DEBUG_XMLRPC
    qDebug() << this << "readRequestBody(): already read" << requestBody.size()
        << "contentLength" << requestHeader.contentLength();
    #endif
    if ( requestBody.size() == ( int ) requestHeader.contentLength() )
        return true;
    else
        return false;
}

bool HttpServer::requestContainsBody()
{
    #ifdef DEBUG_XMLRPC
    qDebug() << this << "requestContainsBody()";
    #endif
    Q_ASSERT( requestHeader.isValid() );
    return requestHeader.hasContentLength() && requestHeader.hasContentLength();
}

void HttpServer::slotBytesWritten( qint64 bytes )
{
    #ifdef DEBUG_XMLRPC
    qDebug() << this << "slotBytesWritten():" << bytes;
    #endif
    bytesWritten += bytes;
    if ( bytesWritten == bytesToWrite )
        {
            state= Done;
            emit    replySent( this );
        }
}

void HttpServer::slotSendReply( const QByteArray &body )
{
    #ifdef DEBUG_XMLRPC
    qDebug() << this << "sendReply():" << body;
    #endif
    Q_ASSERT( state == WaitingReply );
    state = SendingReply;

    /*
     * QByteArray body = toXmlRpcResponse( e );
     */
    QHttpResponseHeader h( xmlRpcResponseHeader( body.size()) );
    QByteArray          hb= h.toString().toLatin1();
    bytesToWrite= hb.size() + body.size();
    bytesWritten= 0;
    socket->write( hb );
    socket->write( body );
    socket->flush();
}

void HttpServer::slotSendReply( const QVariant &e )
{
    #ifdef DEBUG_XMLRPC
    qDebug() << this << "sendReply():" << e;
    #endif
    Q_ASSERT( state == WaitingReply );
    state= SendingReply;

    QByteArray          body= toXmlRpcResponse( e );
    QHttpResponseHeader h( xmlRpcResponseHeader( body.size()) );
    QByteArray          hb = h.toString().toLatin1();
    bytesToWrite = hb.size() + body.size();
    bytesWritten = 0;
    socket->write( hb );
    socket->write( body );
    socket->flush();
}

XmlRpcServer::XmlRpcServer( QObject *parent, const QString &cert, const QString &key, const QByteArray & ) :
    QTcpServer( parent )
{
    #ifdef DEBUG_XMLRPC
    qDebug() << this << "XmlRpcServer(): parent" << parent;
    #endif
    #ifndef QT_NO_OPENSSL
    sslParams= NULL;
    if ( !cert.isEmpty() && !key.isEmpty() ) sslParams= new SslParams( cert, key, this );
    #endif
}

void XmlRpcServer::incomingConnection( int socketDescriptor )
{
    #ifdef DEBUG_XMLRPC
    qDebug() << this << "new incoming connection";
    #endif

    QAbstractSocket *s = NULL;
    #ifndef QT_NO_OPENSSL
    if ( sslParams != NULL && !sslParams->certificate.isNull() )
        {
            s = new QSslSocket( this );
            s->setSocketDescriptor( socketDescriptor );
            ( ( QSslSocket * ) s )->setLocalCertificate( sslParams->certificate );
            ( ( QSslSocket * ) s )->setPrivateKey( sslParams->privateKey );
            ( ( QSslSocket * ) s )->setCaCertificates( sslParams->ca );
        }
    else
        {
            s = new QTcpSocket( this );
            s->setSocketDescriptor( socketDescriptor );
        }

    #else
    s = new QTcpSocket( this );
    s->setSocketDescriptor( socketDescriptor );
    #endif
    Q_ASSERT( s->state() == QAbstractSocket::ConnectedState );
    connect( s, SIGNAL( disconnected()), this, SLOT( slotSocketDisconnected()) );

    HttpServer * p = new HttpServer( s );
    connect( p, SIGNAL( protocolTimeout ( Protocol * )), this, SLOT( slotProtocolTimeout ( Protocol * )) );
    connect( p, SIGNAL( parseError ( HttpServer * )), this, SLOT( slotParseError ( HttpServer * )) );
    connect( p, SIGNAL( requestReceived( HttpServer *, const QHttpRequestHeader &, const QByteArray &)), this,
             SLOT( slotRequestReceived( HttpServer *, const QHttpRequestHeader &, const QByteArray &)) );
    connect( p, SIGNAL( replySent ( HttpServer * )), this, SLOT( slotReplySent ( HttpServer * )) );
}

void XmlRpcServer::slotSocketDisconnected()
{
    #ifdef DEBUG_XMLRPC
    qDebug() << this << "slotSocketDisconnected()";
    #endif

    /* just delete this socket. */
    sender()->deleteLater();
}

void XmlRpcServer::slotProtocolTimeout( Protocol *p )
{
    #ifdef DEBUG_XMLRPC
    qDebug() << this << "slotProtocolTimeout()";
    #endif
    qWarning() << this << "slotProtocolTimeout(): client"
               << p->getSocket()->peerAddress() << ":"
               << p->getSocket()->peerPort() << "closing connection";
    p->getSocket()->close();
}

void XmlRpcServer::slotParseError( HttpServer *p )
{
    #ifdef DEBUG_XMLRPC
    qDebug() << this << "slotParseError()";
    #endif
    qWarning() << this << "slotParseError(): client"
               << p->getSocket()->peerAddress() << ":"
               << p->getSocket()->peerPort()
               << "closing connection";
    p->getSocket()->close();
}

void XmlRpcServer::registerSlot( QObject *receiver, const char *slot, QByteArray path )
{
    #ifdef DEBUG_XMLRPC
    qDebug() << this << "registerSlot():" << receiver << slot;
    #endif

    /* skip code first symbol 1 from SLOT macro (from qobject.cpp) */
    ++slot;

    /* find QMetaMethod.. */
    const QMetaObject   *mo= receiver->metaObject();

    /* we need buf in memory for const char (from qobject.cpp) */
    QByteArray          method= QMetaObject::normalizedSignature( slot );
    const char          *normalized= method.constData();
    int                 slotId= mo->indexOfSlot( normalized );
    if ( slotId == -1 )
      {
        qCritical() << this << "registerSlot():" << receiver << normalized << "can't find slot";
        qFatal( "programming error" );
      }

    QMetaMethod m= receiver->metaObject()->method( slotId );
    bool        isDefferedResult= !qstrcmp( m.typeName(), "DeferredResult*" );
    if ( qstrcmp( m.typeName(), "QVariant") && !isDefferedResult )
      {
        qCritical()
            << this << "registerSlot():" << receiver << normalized <<
                "rpc return type should be QVariant or DeferredResult*, but" << m.typeName();
        qFatal( "programming error" );
      }

    foreach( QByteArray c, m.parameterTypes() ) if ( c != "QVariant" )
      {
        qCritical() << this << "registerSlot():" << receiver << normalized << "all parameters should be QVariant";
        qFatal( "programming error" );
      }

    /* ok, now lets make just function name from our SLOT */
    Q_ASSERT( method.indexOf( '(') > 0 );
    method.truncate( method.indexOf( '(') );
    if ( path[0] != '/' ) path.prepend( '/' );
    if ( path[path.size() - 1] != '/' ) path.append( '/' );
    method.prepend( path );

    /* check if allready exists */
    if ( callbacks.contains( method) )
      {
        qCritical()
            << this << "registerSlot():" << receiver << method << "allready registered, receiver" << callbacks[method];
        qFatal( "programming error" );
      }

    callbacks[method]= IsMethodDeffered( receiver, isDefferedResult );

    Methods &methods= objectMethods[receiver];
    if ( methods.isEmpty() )
      {
        #ifdef DEBUG_XMLRPC
        qDebug() << this << "registerSlot(): connecting SIGNAL(destroyed()) of" << receiver;
        #endif
        connect( receiver, SIGNAL( destroyed ( QObject * )), this, SLOT( slotReceiverDestroed ( QObject * )) );
      }

    methods.append( method );
    #ifdef DEBUG_XMLRPC
    qDebug() << this << "registerSlot(): callbacks" << callbacks;
    qDebug() << this << "registerSlot(): objectMethods" << objectMethods;
    #endif
}

void XmlRpcServer::slotReceiverDestroed( QObject *o )
{
    #ifdef DEBUG_XMLRPC
    qDebug() << this << "slotReceiverDestroed():" << o;
    #endif
    Q_ASSERT( objectMethods.contains( o) );
    foreach ( const char *m, objectMethods[o] ) if ( !callbacks.remove( m) )
        qCritical() << this << "slotReceiverDestroed():" << "can't remove" << m << "from callbacks";
    objectMethods.remove( o );
    #ifdef DEBUG_XMLRPC
    qDebug() << this << "slotReceiverDestroed(): callbacks" << callbacks;
    qDebug() << this << "slotReceiverDestroed(): objectMethods" << objectMethods;
    #endif
}

void XmlRpcServer::slotRequestReceived( HttpServer *p, const QHttpRequestHeader &h, const QByteArray &body )
{
    #ifdef DEBUG_XMLRPC
    qDebug() << this << "slotRequestReceived():" << p << h.toString() << body;
    #endif

    QDomDocument    doc;
    QString         errorMsg;
    int             errorLine, errorColumn;
    if ( !doc.setContent( body, &errorMsg, &errorLine, &errorColumn) )
      {
        qCritical()
            << this << "slotRequestReceived(): can't parse xml" << endl << body << endl << errorMsg << endl <<
                "line:" << errorLine << "column:" << errorColumn;
        p->getSocket()->close();
        return;
      }

    QVariantList    params;
    QDomNode        n;
    QDomElement     e;

    /* run until first element */
    n= doc.firstChild();
    while ( !n.isNull() )
      {
        e= n.toElement();
        if ( !e.isNull() ) break;
        n= n.nextSibling();
      }

    if ( e.tagName() != "methodCall" )
      {
        qCritical() << this << "slotRequestReceived(): first element is not methodCall" << e.tagName() << endl << body;
        p->getSocket()->close();
        return;
      }

    /* first child SHOULD be methodName */
    e= e.firstChild().toElement();
    if ( e.tagName() != "methodName" )
      {
        qCritical() << this << "slotRequestReceived(): first element of methodCall is not methodName" << e.tagName()
            << endl << body;
        p->getSocket()->close();
        return;
      }

    QByteArray  methodName= e.firstChild().toText().data().toLatin1();
    if ( methodName.isEmpty() )
      {
        qCritical() << this << "slotRequestReceived(): methodName empty" << endl << body;
        p->getSocket()->close();
        return;
      }

    QByteArray  path= h.path().toLatin1();
    if ( path[0] != '/' ) path.prepend( '/' );
    if ( path[path.size() - 1] != '/' ) path.append( '/' );
    if ( !callbacks.contains( path + methodName) )
      {
        qCritical() << this << "slotRequestReceived(): method" << path + methodName << "is not registered as callback";
        p->slotSendReply( toXmlRpcResponse( createFault( -1, "method " + path + methodName + " not registered")) );
        return;
      }

    /* run over params */
    e= e.nextSibling().toElement();
    if ( e.tagName() != "params" )
      {
        qCritical()
            << this << "slotRequestReceived(): method" << methodName << "no params element in xml request" << endl <<
                body;
        p->slotSendReply( toXmlRpcResponse( createFault( -1, "no params element in xml request")) );
        return;
      }

    /* run until first element */
    e= e.firstChild().toElement();
    while ( !e.isNull() )
      {
        if ( e.tagName() != "param" )
          {
            qCritical()
                << this << "slotRequestReceived(): method" << methodName << "no param element in xml request" << endl <<
                    body;
            p->slotSendReply( toXmlRpcResponse( createFault( -1, "bad param element in xml request")) );
            return;
          }

        QString err;
        params.append( parseXmlRpcValue( e.firstChild().toElement(), err) );
        if ( !err.isEmpty() )
          {
            qCritical()
                << this << "slotRequestReceived(): method" << methodName << "parse error:" << err << endl << body;
            p->slotSendReply( toXmlRpcResponse( createFault( -1, err)) );
            return;
          }

        e= e.nextSibling().toElement();
      }

    QAbstractSocket *s= p->getSocket();
    #ifndef QT_NO_OPENSSL
    if ( s->inherits( "QSslSocket") )
      {
        QSslSocket  *sslSkt= qobject_cast < QSslSocket * > ( s );
        QString     commonName= sslSkt->peerCertificate().subjectInfo( QSslCertificate::CommonName );
        params.append( commonName );
      }
    #endif

    /* params parsed, now invoke registered slot. */
    QVariant                retVal;
    DeferredResult          *retDeffered;
    const IsMethodDeffered  &md= callbacks[path + methodName];
    bool                    invoked;
    if ( md.second )
      {
        invoked= QMetaObject::invokeMethod( md.first, methodName.constData(), Qt::DirectConnection,
                                            Q_RETURN_ARG( DeferredResult *, retDeffered),
                                            params.size() > 0 ? Q_ARG( QVariant, params[0]) : QGenericArgument(),
                                            params.size() > 1 ? Q_ARG( QVariant, params[1]) : QGenericArgument(),
                                            params.size() > 2 ? Q_ARG( QVariant, params[2]) : QGenericArgument(),
                                            params.size() > 3 ? Q_ARG( QVariant, params[3]) : QGenericArgument(),
                                            params.size() > 4 ? Q_ARG( QVariant, params[4]) : QGenericArgument(),
                                            params.size() > 5 ? Q_ARG( QVariant, params[5]) : QGenericArgument(),
                                            params.size() > 6 ? Q_ARG( QVariant, params[6]) : QGenericArgument(),
                                            params.size() > 7 ? Q_ARG( QVariant, params[7]) : QGenericArgument(),
                                            params.size() > 8 ? Q_ARG( QVariant, params[8]) : QGenericArgument(),
                                            params.size() > 9 ? Q_ARG( QVariant, params[9]) : QGenericArgument() );
      }
    else
      {
        invoked= QMetaObject::invokeMethod( md.first, methodName.constData(), Qt::DirectConnection,
                                            Q_RETURN_ARG( QVariant, retVal),
                                            params.size() > 0 ? Q_ARG( QVariant, params[0]) : QGenericArgument(),
                                            params.size() > 1 ? Q_ARG( QVariant, params[1]) : QGenericArgument(),
                                            params.size() > 2 ? Q_ARG( QVariant, params[2]) : QGenericArgument(),
                                            params.size() > 3 ? Q_ARG( QVariant, params[3]) : QGenericArgument(),
                                            params.size() > 4 ? Q_ARG( QVariant, params[4]) : QGenericArgument(),
                                            params.size() > 5 ? Q_ARG( QVariant, params[5]) : QGenericArgument(),
                                            params.size() > 6 ? Q_ARG( QVariant, params[6]) : QGenericArgument(),
                                            params.size() > 7 ? Q_ARG( QVariant, params[7]) : QGenericArgument(),
                                            params.size() > 8 ? Q_ARG( QVariant, params[8]) : QGenericArgument(),
                                            params.size() > 9 ? Q_ARG( QVariant, params[9]) : QGenericArgument() );
      }

    if ( !invoked )
      {
        qCritical() << this << "slotRequestReceived(): can't invoke" << methodName << params;

        QVariant    fault= createFault( -1, "can't invoke " + path + methodName + ", wrong parameters number?" );
        p->slotSendReply( toXmlRpcResponse( fault) );
        return;
      }

    if ( md.second )
      {
        retDeffered->setParent( p->getSocket() );
        connect( retDeffered, SIGNAL( sendReply ( const QVariant & )), p, SLOT( slotSendReply ( const QVariant & )) );
      } else p->slotSendReply( toXmlRpcResponse( retVal) );
    return;
}

void XmlRpcServer::slotReplySent( HttpServer *p )
{
    #ifdef DEBUG_XMLRPC
    qDebug() << this << "slotReplySent():" << p;
    #endif
    p->getSocket()->close();
}

DeferredEcho::DeferredEcho( const QVariant &e ) :
    echo( e )
{
    startTimer( 1000 ); /* one second timeout, before echo */
}

void DeferredEcho::timerEvent( QTimerEvent * )
{
    emit    sendReply( echo );
}

DeferredResult::DeferredResult()
{
    #ifdef DEBUG_XMLRPC
    qDebug() << this << "DeferredResult()";
    #endif
}

DeferredResult::~DeferredResult()
{
    #ifdef DEBUG_XMLRPC
    qDebug() << this << "~DeferredResult()";
    #endif
}

#include "moc_xmlrpcserver.cpp"
