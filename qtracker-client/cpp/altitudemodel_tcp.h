#ifndef ALTITUDEMODEL_TCPSOCKET_H
#define ALTITUDEMODEL_TCPSOCKET_H

#include <QStringList>
#include <QUrl>
#include <QObject>
#include <QTimer>

#include "interface/commandcaller.h"

class PrivateAltitudeModel : public CommandCaller
{
    Q_OBJECT

public:
    static PrivateAltitudeModel& Instance();

    int    mask()              { return _mask; }
    double current()           { return _current; }
    double average()           { return _average; }
    double maximum()           { return _maximum; }
    double minimum()           { return _minimum; }
    double monitor()           { return _monitor; }
    double ascent()            { return _ascent;  }
    double descent()           { return _descent; }
    int    count()             { return _count; }
    double hysteresis()        { return _hysteresis; }
    void setCount(int)         {}
    void setHysteresis(double) {}

    ~PrivateAltitudeModel();

signals:
    void dataChanged();
    void countChanged();
    void hysteresisChanged();

public slots:
    void resetData();
    void requestData();

protected:
    virtual void commandExecuted(Command* cmd);
    virtual void commandFailed(QAbstractSocket::SocketError error);

private:
    explicit PrivateAltitudeModel(QObject *parent = 0);

private:
    static PrivateAltitudeModel* instance;
    QTimer _timer;
    double _hysteresis;
    int    _count;
    int    _mask;
    double _current;
    double _average;
    double _minimum;
    double _maximum;
    double _ascent;
    double _descent;
    double _monitor;
    Q_DISABLE_COPY(PrivateAltitudeModel)
};

#endif // ALTITUDEMODEL_TCPSOCKET_H
