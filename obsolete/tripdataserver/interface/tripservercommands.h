#ifndef TRIPSEVERCOMMANDS_H
#define TRIPSEVERCOMMANDS_H

#include "command.h"

const quint16 cmdStop =                     0x101;
const quint16 cmdReset =                    0x102;
const quint16 cmdStartTrack =               0x201;
const quint16 cmdStopTrack =                0x202;

const quint16 cmdRequestTripInterfaceData = 0x103;

const quint16 cmdRequestTrackData =    0x203;
const quint16 cmdRequestTripData =     0x301;

const quint16 cmdRequestAltitudeData = 0x302;
const quint16 cmdRequestSpeedData =    0x303;
const quint16 cmdRequestCourseData =   0x304;
const quint16 cmdRequestLocationData = 0x305;
const quint16 cmdRequestTimeData =     0x306;
const quint16 cmdRequestDistanceData = 0x307;

const quint16 cmdResetAltitudeData =   0x312;
const quint16 cmdResetSpeedData =      0x313;
const quint16 cmdResetCourseData =     0x314;
const quint16 cmdResetLocationData =   0x315;
const quint16 cmdResetTimeData =       0x316;
const quint16 cmdResetDistanceData =   0x317;

class StopCommand:       public Command { public: StopCommand() :      Command(cmdStop)      {} };
class ResetCommand:      public Command { public: ResetCommand() :     Command(cmdReset)     {} };
class StopTrackCommand:  public Command { public: StopTrackCommand() : Command(cmdStopTrack) {} };

class ResetAltitudeData: public Command { public: ResetAltitudeData(): Command(cmdResetAltitudeData) {} };
class ResetSpeedData:    public Command { public: ResetSpeedData():    Command(cmdResetSpeedData)    {} };
class ResetCourseData:   public Command { public: ResetCourseData():   Command(cmdResetCourseData)   {} };
class ResetLocationData: public Command { public: ResetLocationData(): Command(cmdResetLocationData) {} };
class ResetTimeData:     public Command { public: ResetTimeData():     Command(cmdResetTimeData)     {} };
class ResetDistanceData: public Command { public: ResetDistanceData(): Command(cmdResetDistanceData) {} };


class StartTrackCommand: public Command
{
public:
    StartTrackCommand(int track, int interval);
    int track();
    int interval();
private:
    int _track;
    int _interval;
};

class RequestTrackData: public Command
{
public:
    RequestTrackData();
    bool valid();
    int  track();
    int  interval();
    void set ( int mask, int track, int interval );
};

class RequestTripInterfaceData: public Command
{
public:
    RequestTripInterfaceData();
    bool    valid();
    int     trip();
    int     track();
    QString tripname();
    QString trackname();
    QString trackstate();
    void set ( int trip, int track, QString tripname, QString trackname, QString trackstate );
};

class RequestTripData: public Command
{
public:
    RequestTripData();
    bool   valid();
    int    trip();
    int    altmask();
    double altcur();
    double altavg();
    double altmin();
    double altmax();
    double ascent();
    double descent();
    int    spdmask();
    double spdcur();
    double spdavg();
    double spdmin();
    double spdmax();
    double distance();
    QString tripname();
    QString trackstate();
    QString trackname();
    QTime  time();
    void set (  int trip,
                int altmask, double altcur, double altavg, double altmin, double altmax, double ascent, double descent,
                int spdmask, double spdcur, double spdavg, double spdmin, double spdmax,
                double distance, const QTime& time, const QString& tripname, const QString& trackstate, const QString& trackname );
};

class RequestAltitudeData: public Command
{
public:
    RequestAltitudeData();
    bool   valid();
    int    mask();
    double current();
    double average();
    double minimum();
    double maximum();
    double ascent();
    double descent();
    double monitor();
    void set ( int mask, double current, double average, double minimum, double maximum, double ascent, double descent, double monitor );
};

class RequestSpeedData: public Command
{
public:
    RequestSpeedData();
    bool   valid();
    int    mask();
    double current();
    double average();
    double minimum();
    double maximum();
    double monitor();
    void set ( int mask, double current, double average, double minimum, double maximum);
};

class RequestCourseData: public Command
{
public:
    RequestCourseData();
    bool   valid();
    int    mask();
    double current();
    double average();
    double monitor();
    void set ( int mask, double current, double average, double monitor );
};

class RequestLocationData: public Command
{
public:
    RequestLocationData();
    bool   valid();
    int    mask();
    double latitude();
    double longitude();
    double altitude();
    void set ( int mask, double longitude, double latitude, double altitude );
};

class RequestTimeData: public Command
{
public:
    RequestTimeData();
    bool valid();
    int  mask();
    QDateTime current();
    QDateTime elapsed();
    QDateTime monitor();
    void set ( int mask, QDateTime current, QDateTime elapsed, QDateTime monitor );
};

class RequestDistanceData: public Command
{
public:
    RequestDistanceData();
    bool   valid();
    int    mask();
    double current();
    double monitor();
    void set ( int mask, double current, double monitor );
};

#endif // TRIPSEVERCOMMANDS_H
