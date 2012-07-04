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

void DistanceData::registerApi(XmlRpcServer* srv)
{
    srv->registerSlot( this, SLOT( data() ),                                  "/RPC2/distance/" );
    srv->registerSlot( this, SLOT( reset() ),                                 "/RPC2/distance/" );
    srv->registerSlot( this, SLOT( setHysteresis(const QVariant& ) ),         "/RPC2/distance/" );
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
            emit distanceChanged(current);
        }

        double delta = _p.distanceTo(c);
        if (delta > _hysteresis)
        {
            current += (delta/1000);
            _p = c;
            emit distanceChanged(current);
        }
    }
    //qDebug() << "DistanceData::onPositionChanged()" << current << monitor;
}

QVariant DistanceData::data()
{
    QMap<QString, QVariant> r;
    r["mask"]    = mask;
    r["current"] = current;
    r["monitor"] = monitor;
    return r;
}
