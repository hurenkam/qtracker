#ifndef DISTANCEMODEL_TCPSOCKET_H
#define DISTANCEMODEL_TCPSOCKET_H

#include <QStringList>
#include <QUrl>
#include <QObject>
#include <QTimer>

#include "interface/commandcaller.h"

class PrivateDistanceModel : public CommandCaller
{
    Q_OBJECT

public:
    static PrivateDistanceModel& Instance();

    int    mask()              { return _mask; }
    double current()           { return _current; }
    double monitor()           { return _monitor; }

    ~PrivateDistanceModel();

signals:
    void dataChanged();

public slots:
    void resetData();
    void requestData();

protected:
    virtual void commandExecuted(Command* cmd);
    virtual void commandFailed(QAbstractSocket::SocketError error);

private:
    explicit PrivateDistanceModel(QObject *parent = 0);

private:
    static PrivateDistanceModel* instance;
    QTimer _timer;
    int    _mask;
    double _current;
    double _monitor;
    Q_DISABLE_COPY(PrivateDistanceModel)
};

#endif // DISTANCEMODEL_TCPSOCKET_H
