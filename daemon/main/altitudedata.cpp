#include <QVariant>
#include <QDebug>
#include "altitudedata.h"

AltitudeData::AltitudeData(QObject *parent)
  : QObject(parent)
  , mask(0)
  , current(0.0)
  , average(0.0)
  , minimum(0.0)
  , maximum(0.0)
  , ascent (0.0)
  , descent(0.0)
  , monitor(0.0)
  , _count(60)
  , _hysteresis(100)
  , _previous(0)
{}

void AltitudeData::registerApi(XmlRpcServer* srv)
{
    srv->registerSlot( this, SLOT( data() ),                                  "/RPC2/altitude/" );
    srv->registerSlot( this, SLOT( reset() ),                                 "/RPC2/altitude/" );
    srv->registerSlot( this, SLOT( setHysteresis(const QVariant& ) ),         "/RPC2/altitude/" );
    srv->registerSlot( this, SLOT( setBufferSize(const QVariant& ) ),         "/RPC2/altitude/" );
}

QVariant AltitudeData::data()
{
    QMap<QString, QVariant> r;
    r["mask"]    = mask;
    r["current"] = current;
    r["average"] = average;
    r["minimum"] = minimum;
    r["maximum"] = maximum;
    r["ascent"]  = ascent;
    r["descent"] = descent;
    r["monitor"] = monitor;
    return r;
}

void AltitudeData::onPositionChanged(double lat, double lon, double alt)
{
    current = alt;
    mask |= CURMASK;
    updateAverage();
    updateMinMax();
    updateAscentDescent();
    //qDebug() << "AltitudeData::onPositionChanged()" << current << average << minimum << maximum << ascent << descent << monitor;
}

void AltitudeData::updateAverage()
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

void AltitudeData::updateMinMax()
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

void AltitudeData::updateAscentDescent()
{
    if (!(mask & ASCMASK))
    {
        _previous = current;
        ascent = 0;
        descent = 0;
        emit ascentChanged(ascent);
        emit descentChanged(descent);
    }

    double delta = current - _previous;
    if (delta > _hysteresis)
    {
        ascent += delta;
        _previous = current;
        emit ascentChanged(ascent);
    }
    if (-delta > _hysteresis)
    {
        descent += -delta;
        _previous = current;
        emit descentChanged(descent);
    }

    mask |= ASCMASK;
    mask |= DESMASK;
}
