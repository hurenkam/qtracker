#ifndef XQLOCATION_STUB_P_H_
#define XQLOCATION_STUB_P_H_

// INCLUDES
//#include "qobject_p.h"
#include <QMainWindow>
#include "qobject.h"
#include "xqlocation.h"
#include "locationsimulator.h"

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

public slots:
    void emitPositionUpdates();

public: // Data
    int                          updateInterval;
    XQLocation::DeviceStatus     status;
    XQLocation::DataQuality      dataQuality;
    int                          error;
private:
    XQLocation* iParent;
    LocationSimulator w;
};

#endif /*XQLOCATION_S60_STUB_P_H_*/

// End of file
