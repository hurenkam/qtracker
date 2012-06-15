#include "speedprovider.h"
#include "interface/helpers.h"

void SpeedProvider::updateAverage()
{
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
    _validmask |= AVGMASK;
    emit averageChanged(_average);
    LOG( << "SpeedProvider::updateAverage(): " << _current << _average )
}

void SpeedProvider::updateMinMax()
{
    if ((!(_validmask & MINMASK)) || (_current<_minimum))
    {
        _minimum = _current;
        _validmask |= MINMASK;
        emit minimumChanged(_minimum);
    }

    if ((!(_validmask & MAXMASK)) || (_current>_maximum))
    {
        _maximum = _current;
        _validmask |= MAXMASK;
        emit maximumChanged(_maximum);
    }
    LOG( "SpeedProvider::updateMinMax(): " << _minimum << _maximum )
}

void SpeedProvider::onSpeedChanged(double speed)
{
    LOG( "SpeedProvider::onSpeedChanged()> " << _validmask )

    _current = speed;
    _validmask |= CURMASK;
    emit currentChanged(_current);

    updateAverage();
    updateMinMax();

    emit maskChanged(_validmask);
    LOG( "SpeedProvider::onSpeedChanged()< " << _validmask )
}

