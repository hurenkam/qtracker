#include <QDebug>
#include "speeddata.h"

SpeedData::SpeedData(QObject *parent)
  : QObject(parent)
  , mask(0)
  , current(0.0)
  , average(0.0)
  , minimum(0.0)
  , maximum(0.0)
  , _count(60)
{
}

void SpeedData::registerApi(XmlRpcServer* srv)
{
    srv->registerSlot( this, SLOT( data() ),                                  "/RPC2/speed/" );
    srv->registerSlot( this, SLOT( reset() ),                                 "/RPC2/speed/" );
    srv->registerSlot( this, SLOT( setBufferSize(const QVariant& ) ),         "/RPC2/speed/" );
}

QVariant SpeedData::data()
{
    QMap<QString, QVariant> r;
    r["mask"]    = mask;
    r["current"] = current;
    r["average"] = average;
    r["minimum"] = minimum;
    r["maximum"] = maximum;
    return r;
}

void SpeedData::onSpeedChanged(double s)
{
    current = s;
    mask |= CURMASK;
    updateAverage();
    updateMinMax();
    //qDebug() << "SpeedData::onSpeedChanged()" << current << average << minimum << maximum;
}

void SpeedData::updateAverage()
{
    _avglist.append(current);
    if (_avglist.count() > _count)
    {
        average -= _avglist[0]/_count;
        average += current/_count;
        _avglist.removeAt(0);
        _avglist.append(current);
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
    emit averageChanged(average);
    mask |= AVGMASK;
}

void SpeedData::updateMinMax()
{
    if ((!(mask & MINMASK)) || (current<minimum))
    {
        minimum = current;
        mask |= MINMASK;
        emit minimumChanged(minimum);
    }

    if ((!(mask & MAXMASK)) || (current>maximum))
    {
        maximum = current;
        mask |= MAXMASK;
        emit maximumChanged(maximum);
    }
}
