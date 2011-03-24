#include <QDebug>
#include <QVariant>
#include "speedserver.h"

#define ENABLE_DEBUG
#include "helpers.h"

SpeedServer::SpeedServer(QObject *parent)
    : CommandHandler(parent)
    , p(0)
    , _count(60)
{
    ENTER("")
    methods << "reset" << "setcount";
    EXIT("")
}

void SpeedServer::onCommand(int method, QVariantList args)
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
    }

    EXIT("")
}

void SpeedServer::setCount(int c)
{
    ENTER("")
    if (c<2) c=2;
    while (c < _avglist.count())
       _avglist.removeFirst();
    _count = c;
    EXIT("")
}

SpeedServer::~SpeedServer()
{
    ENTER("")
    if (p) stop();
    EXIT("")
}

void SpeedServer::onSpeedChanged(double speed)
{
    ENTER("")

    UpdateCurrent(speed);
    UpdateMinMax();
    UpdateAverage();

    EXIT("")
}

void SpeedServer::UpdateCurrent(double speed)
{
    ENTER("")

    _current = speed;
    QVariant v = speed;
    p->setValue("current",v);

    EXIT("")
}

void SpeedServer::UpdateMinMax()
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

void SpeedServer::UpdateAverage()
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

void SpeedServer::reset()
{
    ENTER("")
    _avglist.clear();
    UpdateMinMax();
    UpdateAverage();
    EXIT("")
}

void SpeedServer::start()
{
    ENTER("")

    if (p) return;
    p = new QValueSpacePublisher("/server/speed");

    double d = 0.0;
    p->setValue("current",d);
    p->setValue("average",d);
    p->setValue("min",d);
    p->setValue("max",d);

    emit started();

    EXIT("")
}

void SpeedServer::stop()
{
    ENTER("")

    delete p;
    p = 0;
    emit stopped();

    EXIT("")
}
