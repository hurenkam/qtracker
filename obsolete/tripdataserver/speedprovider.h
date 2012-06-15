#ifndef SPEEDPROVIDER_H
#define SPEEDPROVIDER_H

#include <QObject>

#define CURMASK 0x01
#define AVGMASK 0x02
#define MINMASK 0x04
#define MAXMASK 0x08

class SpeedProvider: public QObject
{
    Q_OBJECT

public:
    SpeedProvider() : QObject(), _count(5)
    { onReset(); }

    int    count()      { return _count; }
    int    validmask()  { return _validmask; }
    double current()    { return _current; }
    double average()    { return _average; }
    double minimum()    { return _minimum; }
    double maximum()    { return _maximum; }

signals:
    void maskChanged(int);
    void currentChanged(double);
    void averageChanged(double);
    void minimumChanged(double);
    void maximumChanged(double);

public slots:
    void onSpeedChanged(double speed);
    void onReset()
    { _validmask = 0; _avglist.clear(); }

private:
    void updateAverage();
    void updateMinMax();

private:
    QList<double> _avglist;
    int    _count;
    int    _validmask;
    double _previous;
    double _current;
    double _average;
    double _minimum;
    double _maximum;
};

#endif // SPEEDPROVIDER_H
