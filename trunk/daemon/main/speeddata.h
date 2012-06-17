#ifndef SPEEDDATA_H
#define SPEEDDATA_H

#include <QObject>
#include <QVariant>

#define CURMASK 0x01
#define AVGMASK 0x02
#define MINMASK 0x04
#define MAXMASK 0x08

class SpeedData : public QObject
{
    Q_OBJECT
public:
    explicit SpeedData(QObject *parent = 0);
    
signals:
    
public slots:
    void onSpeedChanged(double s);
    QVariant data()                           { return toVariant(); }
    QVariant reset()                          { return QVariant(0); }
    QVariant setBufferSize(const QVariant& s) { return QVariant(0); }

public:
    QVariant toVariant();

    int     mask;
    double  current;
    double  average;
    double  minimum;
    double  maximum;

private:
    void updateAverage();
    void updateMinMax();

    QList<double> _avglist;
    int           _count;
};

#endif // SPEEDDATA_H
