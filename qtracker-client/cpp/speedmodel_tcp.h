#ifndef SPEEDMODEL_TCPSOCKET_H
#define SPEEDMODEL_TCPSOCKET_H

#include <QStringList>
#include <QUrl>
#include <QObject>
#include <QTimer>

#include "../../tripdataserver/commandcaller.h"

class PrivateSpeedModel : public CommandCaller
{
    Q_OBJECT

public:
    static PrivateSpeedModel& Instance();

    int    mask()              { return _mask; }
    double current()           { return _current; }
    double average()           { return _average; }
    double maximum()           { return _maximum; }
    double minimum()           { return _minimum; }
    int    count()             { return _count; }
    void setCount(int)         {}

    ~PrivateSpeedModel();

signals:
    void dataChanged();
    void countChanged();

public slots:
    void resetData();
    void requestData();

protected:
    virtual void commandExecuted(Command* cmd);
    virtual void commandFailed(QAbstractSocket::SocketError error);

private:
    explicit PrivateSpeedModel(QObject *parent = 0);

private:
    static PrivateSpeedModel* instance;
    QTimer _timer;
    double _hysteresis;
    int    _count;
    int    _mask;
    double _current;
    double _average;
    double _minimum;
    double _maximum;
    Q_DISABLE_COPY(PrivateSpeedModel)
};

#endif // SPEEDMODEL_TCPSOCKET_H
