#include <QVariant>
#include <QDebug>
#include "distancedata.h"

DistanceData::DistanceData(QObject *parent)
  : QObject(parent)
  , mask(0)
  , current(0.0)
  , monitor(0.0)
  , _hysteresis(30)
{
}

void DistanceData::onPositionChanged(const QGeoPositionInfo& info)
{
    if (info.isValid())
    {
        QGeoCoordinate c = info.coordinate();
        if (!_p.isValid())
        {
            _p = c;
            current = 0;
        }

        double delta = _p.distanceTo(c);
        if (delta > _hysteresis)
        {
            current += (delta/1000);
            _p = c;
        }
    }

    qDebug() << "DistanceData::onPositionChanged()" << current << monitor;
}

QVariant DistanceData::toVariant()
{
    QMap<QString, QVariant> r;
    r["mask"]    = mask;
    r["current"] = current;
    r["monitor"] = monitor;
    return r;
}
