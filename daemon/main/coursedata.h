#ifndef COURSEDATA_H
#define COURSEDATA_H

#include <QObject>
#include <QVariant>

#define CURMASK 0x01
#define AVGMASK 0x02

class CourseData : public QObject
{
    Q_OBJECT
public:
    explicit CourseData(QObject *parent = 0);
    
signals:
    
public slots:
    void onCourseChanged(double c);
    void onPositionChanged(double lat, double lon, double alt);
    QVariant data()                           { return toVariant(); }
    QVariant reset()                          { return QVariant(0); }
    QVariant setBufferSize(const QVariant& s) { return QVariant(0); }

public:
    QVariant toVariant();

    int     mask;
    double  current;
    double  average;
    double  monitor;

private:
    void updateAverage();

    QList<double> _avglist;
    int           _count;
};

#endif // COURSEDATA_H
