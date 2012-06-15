#ifndef ALTITUDEPROVIDER_H
#define ALTITUDEPROVIDER_H

#include <QObject>

#define CURMASK 0x01
#define AVGMASK 0x02
#define MINMASK 0x04
#define MAXMASK 0x08
#define ASCMASK 0x10
#define DESMASK 0x20

class AltitudeProvider: public QObject
{
    Q_OBJECT

public:
    AltitudeProvider() : QObject(), _count(60), _hysteresis(100)
    { onReset(); }

    int    count()      { return _count; }
    double hysteresis() { return _hysteresis; }
    int    validmask()  { return _validmask; }
    double current()    { return _current; }
    double average()    { return _average; }
    double minimum()    { return _minimum; }
    double maximum()    { return _maximum; }
    double ascent()     { return _ascent; }
    double descent()    { return _descent; }

signals:
    void hysteresisChanged(double);
    void maskChanged(int);
    void currentChanged(double);
    void averageChanged(double);
    void minimumChanged(double);
    void maximumChanged(double);
    void ascentChanged(double);
    void descentChanged(double);

public slots:
    void onAltitudeChanged(double altitude);
    void onReset()
    { _validmask = 0; _ascent = 0; _descent = 0; _avglist.clear(); }

private:
    void updateAverage();
    void updateMinMax();
    void updateAscentDescent();

private:
    QList<double> _avglist;
    int    _count;
    double _hysteresis;
    int    _validmask;
    double _previous;
    double _current;
    double _average;
    double _minimum;
    double _maximum;
    double _ascent;
    double _descent;
};

#endif // ALTITUDEPROVIDER_H
