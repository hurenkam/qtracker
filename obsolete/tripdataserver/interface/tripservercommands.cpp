#include <QTime>
#include "command.h"
#include "tripservercommands.h"

StartTrackCommand::StartTrackCommand(int track, int interval) : Command(cmdStartTrack), _track(track), _interval(interval)
{
    arguments.append(QVariant(track));
    arguments.append(QVariant(interval));
}

int StartTrackCommand::track()
{ return _track; }

int StartTrackCommand::interval()
{ return _interval; }




RequestTripInterfaceData::RequestTripInterfaceData() : Command(cmdRequestTripInterfaceData)
{}

bool RequestTripInterfaceData::valid()
{
    bool r = ((result.type() == QVariant::List) && (result.toList().size()==5));
    //qDebug() << "RequestTripData::valid(): " << r;
    return r;
}

void RequestTripInterfaceData::set ( int tripid, int trackid, QString tripname, QString trackname, QString trackstate)
{
    QVariantList r;
    r.append(QVariant(tripid));
    r.append(QVariant(trackid));
    r.append(QVariant(tripname));
    r.append(QVariant(trackname));
    r.append(QVariant(trackstate));
    result = r;
}

int      RequestTripInterfaceData::trip()       { return valid()? result.toList()  [0].toInt():    0; }
int      RequestTripInterfaceData::track()      { return valid()? result.toList()  [1].toInt():    0; }
QString  RequestTripInterfaceData::tripname()   { return valid()? result.toList()  [2].toString(): "<new>"; }
QString  RequestTripInterfaceData::trackname()  { return valid()? result.toList()  [3].toString(): "<new>"; }
QString  RequestTripInterfaceData::trackstate() { return valid()? result.toList()  [4].toString(): "idle"; }




RequestTripData::RequestTripData() : Command(cmdRequestTripData)
{}

bool RequestTripData::valid()
{
    bool r = ((result.type() == QVariant::List) && (result.toList().size()==18));
    //qDebug() << "RequestTripData::valid(): " << r;
    return r;
}

int     RequestTripData::trip()       { return valid()? result.toList()  [0].toInt():    0; }
int     RequestTripData::altmask()    { return valid()? result.toList()  [1].toInt():    0; }
double  RequestTripData::altcur()     { return valid()? result.toList()  [2].toDouble(): 0; }
double  RequestTripData::altavg()     { return valid()? result.toList()  [3].toDouble(): 0; }
double  RequestTripData::altmin()     { return valid()? result.toList()  [4].toDouble(): 0; }
double  RequestTripData::altmax()     { return valid()? result.toList()  [5].toDouble(): 0; }
double  RequestTripData::ascent()     { return valid()? result.toList()  [6].toDouble(): 0; }
double  RequestTripData::descent()    { return valid()? result.toList()  [7].toDouble(): 0; }
int     RequestTripData::spdmask()    { return valid()? result.toList()  [8].toInt():    0; }
double  RequestTripData::spdcur()     { return valid()? result.toList()  [9].toDouble(): 0; }
double  RequestTripData::spdavg()     { return valid()? result.toList() [10].toDouble(): 0; }
double  RequestTripData::spdmin()     { return valid()? result.toList() [11].toDouble(): 0; }
double  RequestTripData::spdmax()     { return valid()? result.toList() [12].toDouble(): 0; }
double  RequestTripData::distance()   { return valid()? result.toList() [13].toDouble(): 0; }
QTime   RequestTripData::time()       { return valid()? result.toList() [14].toTime():   QTime(0,0,0); }
QString RequestTripData::tripname()   { return valid()? result.toList() [15].toString(): "<noname>"; }
QString RequestTripData::trackstate() { return valid()? result.toList() [16].toString(): "idle"; }
QString RequestTripData::trackname()  { return valid()? result.toList() [17].toString(): "<noname>"; }

void RequestTripData::set (  int trip,
            int altmask, double altcur, double altavg, double altmin, double altmax, double ascent, double descent,
            int spdmask, double spdcur, double spdavg, double spdmin, double spdmax,
            double distance, const QTime& time, const QString& tripname, const QString& trackstate, const QString& trackname )
{
    QVariantList r;
    r.append(QVariant(trip));
    r.append(QVariant(altmask));
    r.append(QVariant(altcur));
    r.append(QVariant(altavg));
    r.append(QVariant(altmin));
    r.append(QVariant(altmax));
    r.append(QVariant(ascent));
    r.append(QVariant(descent));
    r.append(QVariant(spdmask));
    r.append(QVariant(spdcur));
    r.append(QVariant(spdavg));
    r.append(QVariant(spdmin));
    r.append(QVariant(spdmax));
    r.append(QVariant(distance));
    r.append(QVariant(time));
    r.append(QVariant(tripname));
    r.append(QVariant(trackstate));
    r.append(QVariant(trackname));
    result = r;
}




RequestAltitudeData::RequestAltitudeData() : Command(cmdRequestAltitudeData)
{}

bool RequestAltitudeData::valid()
{  return ((result.type() == QVariant::List) && (result.toList().size()==8)); }

int     RequestAltitudeData::mask()       { return valid()? result.toList()  [0].toInt():    0; }
double  RequestAltitudeData::current()    { return valid()? result.toList()  [1].toDouble(): 0; }
double  RequestAltitudeData::average()    { return valid()? result.toList()  [2].toDouble(): 0; }
double  RequestAltitudeData::minimum()    { return valid()? result.toList()  [3].toDouble(): 0; }
double  RequestAltitudeData::maximum()    { return valid()? result.toList()  [4].toDouble(): 0; }
double  RequestAltitudeData::ascent()     { return valid()? result.toList()  [5].toDouble(): 0; }
double  RequestAltitudeData::descent()    { return valid()? result.toList()  [6].toDouble(): 0; }
double  RequestAltitudeData::monitor()    { return valid()? result.toList()  [7].toDouble(): 0; }

void RequestAltitudeData::set ( int mask, double current, double average, double minimum, double maximum, double ascent, double descent, double monitor )
{
    QVariantList r;
    r.append(QVariant(mask));
    r.append(QVariant(current));
    r.append(QVariant(average));
    r.append(QVariant(minimum));
    r.append(QVariant(maximum));
    r.append(QVariant(ascent));
    r.append(QVariant(descent));
    r.append(QVariant(monitor));
    result = r;
}



RequestSpeedData::RequestSpeedData() : Command(cmdRequestSpeedData)
{}

bool RequestSpeedData::valid()
{  return ((result.type() == QVariant::List) && (result.toList().size()==5)); }

int     RequestSpeedData::mask()       { return valid()? result.toList()  [0].toInt():    0; }
double  RequestSpeedData::current()    { return valid()? result.toList()  [1].toDouble(): 0; }
double  RequestSpeedData::average()    { return valid()? result.toList()  [2].toDouble(): 0; }
double  RequestSpeedData::minimum()    { return valid()? result.toList()  [3].toDouble(): 0; }
double  RequestSpeedData::maximum()    { return valid()? result.toList()  [4].toDouble(): 0; }

void RequestSpeedData::set ( int mask, double current, double average, double minimum, double maximum )
{
    QVariantList r;
    r.append(QVariant(mask));
    r.append(QVariant(current));
    r.append(QVariant(average));
    r.append(QVariant(minimum));
    r.append(QVariant(maximum));
    result = r;
}



RequestCourseData::RequestCourseData() : Command(cmdRequestCourseData)
{}

bool RequestCourseData::valid()
{  return ((result.type() == QVariant::List) && (result.toList().size()==4)); }

int     RequestCourseData::mask()       { return valid()? result.toList()  [0].toInt():    0; }
double  RequestCourseData::current()    { return valid()? result.toList()  [1].toDouble(): 0; }
double  RequestCourseData::average()    { return valid()? result.toList()  [2].toDouble(): 0; }
double  RequestCourseData::monitor()    { return valid()? result.toList()  [3].toDouble(): 0; }

void RequestCourseData::set ( int mask, double current, double average, double monitor )
{
    QVariantList r;
    r.append(QVariant(mask));
    r.append(QVariant(current));
    r.append(QVariant(average));
    r.append(QVariant(monitor));
    result = r;
}



RequestLocationData::RequestLocationData() : Command(cmdRequestLocationData)
{}

bool RequestLocationData::valid()
{  return ((result.type() == QVariant::List) && (result.toList().size()==4)); }

int     RequestLocationData::mask()       { return valid()? result.toList()  [0].toInt():    0; }
double  RequestLocationData::latitude()   { return valid()? result.toList()  [1].toDouble(): 0; }
double  RequestLocationData::longitude()  { return valid()? result.toList()  [2].toDouble(): 0; }
double  RequestLocationData::altitude()   { return valid()? result.toList()  [3].toDouble(): 0; }

void RequestLocationData::set ( int mask, double latitude, double longitude, double altitude )
{
    QVariantList r;
    r.append(QVariant(mask));
    r.append(QVariant(latitude));
    r.append(QVariant(longitude));
    r.append(QVariant(altitude));
    result = r;
}



RequestTimeData::RequestTimeData() : Command(cmdRequestTimeData)
{}

bool RequestTimeData::valid()
{  return ((result.type() == QVariant::List) && (result.toList().size()==4)); }

int  RequestTimeData::mask()       { return valid()? result.toList()  [0].toInt():  0; }
QDateTime RequestTimeData::current()    { return valid()? result.toList()  [1].toDateTime(): QDateTime::currentDateTime(); }
QDateTime RequestTimeData::elapsed()    { return valid()? result.toList()  [2].toDateTime(): QDateTime(); }
QDateTime RequestTimeData::monitor()    { return valid()? result.toList()  [3].toDateTime(): QDateTime(); }

void RequestTimeData::set ( int mask, QDateTime current, QDateTime elapsed, QDateTime monitor )
{
    QVariantList r;
    r.append(QVariant(mask));
    r.append(QVariant(current));
    r.append(QVariant(elapsed));
    r.append(QVariant(monitor));
    result = r;
}



RequestDistanceData::RequestDistanceData() : Command(cmdRequestDistanceData)
{}

bool RequestDistanceData::valid()
{  return ((result.type() == QVariant::List) && (result.toList().size()==3)); }

int     RequestDistanceData::mask()       { return valid()? result.toList()  [0].toInt():    0; }
double  RequestDistanceData::current()    { return valid()? result.toList()  [1].toDouble(): 0; }
double  RequestDistanceData::monitor()    { return valid()? result.toList()  [2].toDouble(): 0; }

void RequestDistanceData::set ( int mask, double current, double monitor )
{
    QVariantList r;
    r.append(QVariant(mask));
    r.append(QVariant(current));
    r.append(QVariant(monitor));
    result = r;
}
