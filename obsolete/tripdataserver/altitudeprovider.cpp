#include "altitudeprovider.h"

#include "interface/helpers.h"

void AltitudeProvider::updateAverage()
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
    LOG( "AltitudeProvider::updateAverage(): " << _current << _average )
}

void AltitudeProvider::updateMinMax()
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
    LOG( "AltitudeProvider::updateMinMax(): " << _minimum << _maximum )
}

void AltitudeProvider::updateAscentDescent()
{
    if (!(_validmask & ASCMASK))
    {
        _previous = _current;
        _ascent = 0;
        _descent = 0;
        emit ascentChanged(_ascent);
        emit descentChanged(_descent);
    }

    double delta = _current - _previous;
    if (delta > _hysteresis)
    {
        _ascent += delta;
        _previous = _current;
        emit ascentChanged(_ascent);
    }
    if (-delta > _hysteresis)
    {
        _descent += -delta;
        _previous = _current;
        emit descentChanged(_descent);
    }

    _validmask |= ASCMASK;
    _validmask |= DESMASK;

    LOG( "AltitudeProvider::updateAscentDescent(): " << _ascent << _descent )
}

void AltitudeProvider::onAltitudeChanged(double altitude)
{
    LOG( "AltitudeProvider::onAltitudeChanged()> " << _validmask )

    _current = altitude;
    _validmask |= CURMASK;
    emit currentChanged(_current);

    updateAverage();
    updateMinMax();
    updateAscentDescent();

    emit maskChanged(_validmask);

    LOG( "AltitudeProvider::onAltitudeChanged()< " << _validmask )
}
