#include <QObject>
#include <QVariant>
#include <QDebug>
#include "coursedata.h"

CourseData::CourseData(QObject *parent)
    : QObject(parent)
    , mask(0)
    , current(0.0)
    , average(0.0)
    , monitor(0.0)
    , _count(60)
{
}

QVariant CourseData::toVariant()
{
    QMap<QString, QVariant> r;
    r["mask"]    = mask;
    r["current"] = current;
    r["average"] = average;
    r["monitor"] = monitor;
    return r;
}

void CourseData::onCourseChanged(double c)
{
    mask |= CURMASK;
    current = c;
    updateAverage();
    //qDebug() << "CourseData::onPositionChanged()" << current << average << monitor;
}

void CourseData::onPositionChanged(double lat, double lon, double alt)
{
}

void CourseData::updateAverage()
{
    _avglist.append(current);
    if (_avglist.count() > _count)
    {
        average -= _avglist[0]/_count;
        average += current/_count;
        _avglist.removeAt(0);
    }
    else
    {
        if (_avglist.count()>1)
        {
            average =  average * (_avglist.count()-1)/_avglist.count();
            average += current/_avglist.count();
        }
        else
        {
            average = current;
        }
    }
    mask |= AVGMASK;
}
