#ifndef XQLOCATION_H
#define XQLOCATION_H

// INCLUDES
#include <QObject>
#include "location_global.h"

// FORWARD DECLARATIONS
class XQLocationPrivate;
/*
class XQSatInfo : public QObject
{
	Q_OBJECT
	
public:
	XQSatInfo::XQSatInfo() {}
	XQSatInfo::XQSatInfo(int i, int s, double a, double e)
		: id(i), strength(s), azimuth(a), elevation(e) {}
	double Azimuth()   { return azimuth; }
	double Elevation() { return elevation; }
	int Id()           { return id; }
	int Strength()     { return strength; }
	bool IsUsed()      { return inuse; }
	
private:
	double azimuth;
	double elevation;
	int id;
	int strength;
	bool inuse;
};
*/
// CLASS DECLARATION
class XQLOCATION_EXPORT XQLocation : public QObject
{
    Q_OBJECT

public:
    enum Error {
        NoError = 0,
        OutOfMemoryError,
        OpenError,
        DefaultModuleNotFoundError,
        ModuleNotFoundError,
        InvalidUpdateIntervalError,
        AccessDeniedError,
        InternalError,
        UnknownError = -1
    };

    enum DeviceStatus {
        StatusError,
        StatusDisabled,
        StatusInactive,
        StatusInitialising,
        StatusStandBy,
        StatusReady,
        StatusActive,
        StatusUnknown = -1
    };

    enum DataQuality {
        DataQualityLoss,
        DataQualityPartial,
        DataQualityNormal,
        DataQualityUnknown = -1
    };

    XQLocation(QObject* parent = 0);
    ~XQLocation();
    
    void setUpdateInterval(int interval);
    int updateInterval() const;

    XQLocation::DeviceStatus status() const;
    XQLocation::DataQuality dataQuality() const;
    
    XQLocation::Error open();
    void close();

	//bool GetSatelliteData(int id, XQSatInfo &satinfo);

Q_SIGNALS:
    void locationChanged(double latitude, double longitude, double altitude, float speed, float heading);
    void latitudeChanged(double latitude, float accuracy);
    void longitudeChanged(double longitude, float accuracy);
    void altitudeChanged(double altitude, float accuracy);
    void speedChanged(float speed);
    void headingChanged(float heading);

    void statusChanged(XQLocation::DeviceStatus status);
    void dataQualityChanged(XQLocation::DataQuality dataQuality);
    void numberOfSatellitesInViewChanged(int numSatellites);
    void numberOfSatellitesUsedChanged(int numSatellites);
    
    void updateSatInfo(int id, int strength, double azimuth, double elevation, bool isused);

    void error(XQLocation::Error errorCode);

public Q_SLOTS:
    void requestUpdate();
    void startUpdates();
    void startUpdates(int msec);
    void stopUpdates();

private: // Data
	XQLocationPrivate* d;
	friend class XQLocationPrivate;
};

#endif // XQLOCATION_H

// End of file
