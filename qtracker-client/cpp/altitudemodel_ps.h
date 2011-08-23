#ifndef ALTITUDEMODEL_TCPSOCKET_H
#define ALTITUDEMODEL_TCPSOCKET_H

#include <QStringList>
#include <QUrl>
#include <QObject>
#include <QTimer>

QT_BEGIN_HEADER

class PrivateAltitudeModel : public QObject
{
    Q_OBJECT

public:
    static PrivateAltitudeModel& Instance();

    double current()           { return _current; }
    double average()           { return _average; }
    double ascent()            { return _ascent;  }
    double descent()           { return _descent; }
    double max()               { return _max; }
    double min()               { return _min; }
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
    void onDataRequest();
    void onDataReady();

private:
    explicit PrivateAltitudeModel(QObject *parent = 0);

private:
    static PrivateAltitudeModel* instance;
    QTimer _timer;
    double _hysteresis;
    int    _count;
    double _current;
    double _average;
    double _min;
    double _max;
    double _ascent;
    double _descent;
    Q_DISABLE_COPY(PrivateAltitudeModel)
};

QT_END_HEADER

#endif // ALTITUDEMODEL_TCPSOCKET_H
