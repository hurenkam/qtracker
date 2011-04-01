#include <QDebug>
#include "client.h"

#define ENABLE_DEBUG
#include "helpers.h"

#ifdef Q_OS_SYMBIAN
#include <apacmdln.h>
#include <apgcli.h>
#include <apgtask.h>
#include <e32std.h>
#include <w32std.h>

TBool IsExeRunning( const TDesC& aExeName )
{
//make sure that the name of executable is without extension. i.e helloworld not helloworld.exe
   TFileName executableName;
   executableName.Copy( aExeName );
   executableName.Trim();
   executableName.Append(_L("*"));

   TBool situationFlag = EFalse;
   TFindProcess processSearch;
   TFullName processFullName;
   while( processSearch.Next( processFullName ) == KErrNone)
   {

      if( processFullName.Match(executableName) != KErrNotFound )
      {
         return ETrue;
      }
   }
   return situationFlag;
}
#endif



Client::Client(QObject *parent)
    : QObject(parent)
    , publisher(0)
    , subscriber(0)
{
    ENTER("")
    EXIT("")
}

Q_INVOKABLE void Client::startServer()
{
    ENTER("")

#ifdef Q_OS_SYMBIAN
    QString app = "qtrackerd";
    TPtrC16 textPtr(reinterpret_cast<const TUint16*>(app.utf16()));

    if (!IsExeRunning(textPtr))
    {
        process.start("qtrackerd.exe");
    }
#endif

    if (!publisher)
        publisher  = new QValueSpacePublisher   ("/server/command");
    if (!subscriber)
        subscriber = new  QValueSpaceSubscriber ("/server/response");
    EXIT("")
}

Q_INVOKABLE void Client::sendCommand(const QVariant& value)
{
    ENTER("")
    if (publisher)
        publisher->setValue("cmd",value);
    EXIT("")
}

Q_INVOKABLE QVariant Client::getReply()
{
    ENTER("")
    if (subscriber)
        return subscriber->value("ack");
    else
        return -1;
}

Q_INVOKABLE QVariant Client::getPlatform()
{
    ENTER("")
    int result = -1;
#if   defined(Q_OS_SYMBIAN)
    result = 0;
#elif defined(Q_WS_MAEMO_5)
    result = 1;
#elif defined(Q_WS_SIMULATOR)
    result = 2;
#endif
    return QVariant(result);
}
