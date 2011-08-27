#ifndef DIRECTIONPROVIDER_H
#define DIRECTIONPROVIDER_H

#include <QObject>

#define CURMASK 0x01
#define AVGMASK 0x02

class CourseProvider: public QObject
{
    Q_OBJECT
public:
    CourseProvider()
        : QObject()
        , _count(30)
        , _validmask(0)
    {}

    int    count()      { return _count; }
    int    validmask()  { return _validmask; }
    double current()    { return _current; }
    double average()    { return _average; }

signals:
    void maskChanged(int);
    void currentChanged(double);
    void averageChanged(double);

public slots:
    void onCourseChanged(double course);
    void onReset()
    { _validmask = 0; _avglist.clear(); emit maskChanged(_validmask); }

private:
    void updateAverage();

private:
    QList<double> _avglist;
    int    _count;
    int    _validmask;
    double _current;
    double _average;
};

#endif // DIRECTIONPROVIDER_H
