#ifndef XMLRPCCONV_H
#define XMLRPCCONV_H

#include <QByteArray>
#include <QList>
#include <QVariant>
#include <QtNetwork/QHttpResponseHeader>
#include <QtXml/QDomElement>

QByteArray          toXmlRpcRequest( const QString m, const QList<QVariant> &l );
QVariant            fromXmlRpcResponse( const QString d, QString &err );

/* returns default http header for our xmlrpc server */
QHttpResponseHeader xmlRpcResponseHeader( const qint64 contentLength );

/* create xmlrpc response from QVariant */
QByteArray          toXmlRpcResponse( const QVariant &v );

/*
 * QVariant to xml conversions ;
 * use QByteArray & reference, becouse it is faster, then return QByteArray
 */
void                toXmlRpcValue( const int spaces, const QVariant &child, QByteArray &b );
void                toXmlRpcStruct( const int spaces, const QVariantMap &child, QByteArray &b );
void                toXmlRpcArray( const int spaces, const QVariantList &child, QByteArray &b );

/* xml to QVariant conversions */
QVariant            parseXmlRpcValue( const QDomElement &e, QString &err );
QVariant            parseXmlRpcStruct( const QDomElement &e, QString &err );
QVariant            parseXmlRpcArray( const QDomElement &e, QString &err );

#endif
