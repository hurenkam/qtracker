#ifndef XQLOCATION_STUB_P_H_
#define XQLOCATION_STUB_P_H_

// INCLUDES
//#include "qobject_p.h"
#include "qobject.h"
#include "xqlocation.h"

// CLASS DECLARATION
class XQLocationPrivate: public QObject
{
    //Q_DECLARE_PUBLIC(XQLocation)
    Q_OBJECT

public:
    //XQLocationPrivate();
    XQLocationPrivate(XQLocation* const);
    ~XQLocationPrivate();
    
    void requestUpdate();
    void startUpdates();
    void startUpdates(int msec);
    void stopUpdates();
    
    XQLocation::Error openConnectionToPositioner();
    void closeConnectionToPositioner();

public: // Data
    int                          updateInterval;
    XQLocation::DeviceStatus     status;
    XQLocation::DataQuality      dataQuality;
    int                          error;
};

#endif /*XQLOCATION_S60_STUB_P_H_*/

// End of file
