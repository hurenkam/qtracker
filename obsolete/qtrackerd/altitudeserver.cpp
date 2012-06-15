#include <QDebug>
#include <QVariant>
#include <QStringList>
#include "altitudeserver.h"

//#define ENABLE_DEBUG
#include "helpers.h"

AltitudeServer::AltitudeServer(QObject *parent)
    : CommandHandler(parent)
    , p(0)
    , _hysteresis(25)
    , _previous(0)
    , _count(60)
{
    ENTER("")
    methods << "reset" << "setcount" << "sethysteresis";
    EXIT("")
}
/*
void AltitudeServer::onCommand(QVariant cmd)
{
    ENTER("")

    QVariantMap map = cmd.toMap();
    if (!map.contains("method")) return;

    QString method = map["method"].toString();
    QVariantList args;
    if (map.contains("args"))
        args = map["args"].toList();

    QStringList supported;
    supported << "reset" << "setcount" << "sethysteresis";
    if (!supported.contains(method)) return;

    switch (supported.indexOf(method))

*/

void AltitudeServer::onCommand(int method, QVariantList args)
{
    ENTER("")

    switch (method)
    {
        case 0:
            reset();
            break;
        case 1:
            if (args.count()>0)
                setCount(args[0].toInt());
            break;
        case 2:
            if (args.count()>0)
                setHysteresis(args[0].toDouble());
            break;
    }

    EXIT("")
}

AltitudeServer::~AltitudeServer()
{
    ENTER("")
    if (p) stop();
    EXIT("")
}

void AltitudeServer::setCount(int c)
{
    ENTER("")
    if (c<2) c=2;
    while (c < _avglist.count())
       _avglist.removeFirst();
    _count = c;
    EXIT("")
}

void AltitudeServer::setHysteresis(double h)
{
    ENTER("")
    _hysteresis = h>0? h : 0;
    EXIT("")
}

void AltitudeServer::onAltitudeChanged(double altitude)
{
    ENTER(altitude)

    UpdateCurrent(altitude);
    UpdateAscentDescent();
    UpdateMinMax();
    UpdateAverage();

    EXIT("")
}

void AltitudeServer::UpdateCurrent(double altitude)
{
    ENTER("")

    _current = altitude;
    QVariant v = altitude;
    p->setValue("current",v);

    EXIT("")
}

void AltitudeServer::UpdateAscentDescent()
{
    ENTER("")

    if (!_avglist.count())
    { _previous = _current; return; }

    double delta = _current - _previous;
    QVariant v;
    if (delta > _hysteresis)
    {
        _ascent += delta;
        _previous = _current;
        v = _ascent;
        p->setValue("ascent",v);
        emit ascentChanged(_ascent);
    }
    if ((-1*delta) > _hysteresis)
    {
        _descent -= delta;
        _previous = _current;
        v = _descent;
        p->setValue("descent",v);
        emit descentChanged(_descent);
    }

    EXIT("")
}

void AltitudeServer::UpdateMinMax()
{
    ENTER("")

    QVariant v;
    if (_avglist.count())
    {
        _max = (_current > _max)? _current: _max;
        _min = (_current < _min)? _current: _min;
    }
    else
    {
        _max = _current;
        _min = _current;
    }

    v = _max;
    p->setValue("max",v);
    emit maxChanged(_max);
    v = _min;
    p->setValue("min",v);
    emit minChanged(_min);

    EXIT("")
}

void AltitudeServer::UpdateAverage()
{
    ENTER("")

    QVariant v;

    _avglist.append(_current);
    if (_avglist.count() > _count)
    {
        _average -= _avglist[0]/_count;
        _average += _current/_count;
        _avglist.removeAt(0);
    }
    else
    {
        if (_avglist.count()>1)
        {
            _average =  _average * (_avglist.count()-1)/_avglist.count();
            _average += _current/_avglist.count();
        }
        else
        {
            _average = _current;
        }
    }

    v = _average;
    p->setValue("average",v);
    emit averageChanged(_average);

    EXIT("")
}

void AltitudeServer::reset()
{
    ENTER("")
    _avglist.clear();
    _ascent = 0;
    _descent = 0;
    UpdateMinMax();
    UpdateAverage();
    EXIT("")
}

void AltitudeServer::start()
{
    ENTER("")

    if (p) return;

    p = new QValueSpacePublisher("/server/altitude");

    double d = 0.0;
    p->setValue("current",d);
    p->setValue("average",d);
    p->setValue("ascent",d);
    p->setValue("descent",d);
    p->setValue("min",d);
    p->setValue("max",d);

    emit started();

    EXIT("")
}

void AltitudeServer::stop()
{
    ENTER("")

    delete p;
    p = 0;
    emit stopped();

    EXIT("")
}
