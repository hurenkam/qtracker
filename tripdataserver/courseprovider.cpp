#include "courseprovider.h"
#include "interface/helpers.h"

void CourseProvider::updateAverage()
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
    LOG( << "CourseProvider::updateAverage(): " << _current << _average )
}

void CourseProvider::onCourseChanged(double course)
{
    LOG( "CourseProvider::onCourseChanged()> " << _validmask )

    _current = course;
    _validmask |= CURMASK;
    emit currentChanged(_average);

    updateAverage();

    emit maskChanged(_validmask);
    LOG( "CourseProvider::onCourseChanged()< " << _validmask )
}

