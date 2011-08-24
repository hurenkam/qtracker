#ifndef TRIPSERVERINTERFACE_TCPSOCKET_H
#define TRIPSERVERINTERFACE_TCPSOCKET_H

#include <QStringList>
#include <QUrl>
#include <QObject>
#include <QTimer>

#include "../../tripdataserver/commandcaller.h"

class PrivateTripServerInterface : public CommandCaller
{
    Q_OBJECT

public:
    static PrivateTripServerInterface& Instance();

    int     trip()              { return _trip; }
    int     track()             { return _track; }
    QString trackname()         { return _trackname; }
    QString trackstate()        { return _trackstate; }

    ~PrivateTripServerInterface();

signals:
    void dataChanged();

public slots:
    void requestData();
    void resetServer();
    void stopServer();
    void startTrack(int track, int interval);
    void stopTrack();

protected:
    virtual void commandExecuted(Command* cmd);
    virtual void commandFailed(QAbstractSocket::SocketError error);

private:
    explicit PrivateTripServerInterface(QObject *parent = 0);

private:
    static PrivateTripServerInterface* instance;
    QTimer  _timer;
    int     _trip;
    int     _track;
    QString _trackname;
    QString _trackstate;
    Q_DISABLE_COPY(PrivateTripServerInterface)
};

#endif // TRIPSERVERINTERFACE_TCPSOCKET_H
