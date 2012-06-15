#ifndef TIMEMODEL_TCPSOCKET_H
#define TIMEMODEL_TCPSOCKET_H

#include <QStringList>
#include <QUrl>
#include <QObject>
#include <QTimer>
#include <QDateTime>

#include "interface/commandcaller.h"

class PrivateTimeModel : public CommandCaller
{
    Q_OBJECT

public:
    static PrivateTimeModel& Instance();

    int  mask()                   { return _mask; }
    QDateTime current()           { return _current; }
    QDateTime elapsed()           { return _elapsed; }
    QDateTime monitor()           { return _monitor; }

    ~PrivateTimeModel();

signals:
    void dataChanged();

public slots:
    void resetData();
    void requestData();

protected:
    virtual void commandExecuted(Command* cmd);
    virtual void commandFailed(QAbstractSocket::SocketError error);

private:
    explicit PrivateTimeModel(QObject *parent = 0);

private:
    static PrivateTimeModel* instance;
    QTimer      _timer;
    int         _mask;
    QDateTime   _current;
    QDateTime   _elapsed;
    QDateTime   _monitor;
    Q_DISABLE_COPY(PrivateTimeModel)
};

#endif // TIMEMODEL_TCPSOCKET_H
