#ifndef ALTITUDESERVER_H
#define ALTITUDESERVER_H

#include <QObject>
#include <QValueSpacePublisher>
#include "commandhandler.h"

using namespace QtMobility;

class AltitudeServer : public CommandHandler
{
    Q_OBJECT
public:
    explicit AltitudeServer(QObject *parent = 0);
    ~AltitudeServer();

signals:
    void started();
    void stopped();
    void averageChanged(double);
    void ascentChanged(double);
    void descentChanged(double);
    void minChanged(double);
    void maxChanged(double);

public slots:
    void start();
    void stop();
    void reset();
    void onAltitudeChanged(double);
    void setHysteresis(double h);
    void setCount(int c);

public:
    //virtual void onCommand(QVariant cmd);
    virtual void onCommand(int method, QVariantList args);

private:
    void UpdateCurrent(double altitude);
    void UpdateAscentDescent();
    void UpdateMinMax();
    void UpdateAverage();

private:
    QValueSpacePublisher* p;
    QList<double> _avglist;
    double _hysteresis;
    double _previous;
    int    _count;

    double _current;
    double _ascent;
    double _descent;
    double _average;
    double _max;
    double _min;
};

#endif // ALTITUDESERVER_H
