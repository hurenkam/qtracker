// INCLUDE FILES
#include "xqlocation.h"
#include "xqlocation_stub_p.h"

XQLocationPrivate::XQLocationPrivate(XQLocation * const)
: updateInterval(1000) // default update interval is one second
{
}

XQLocationPrivate::~XQLocationPrivate()
{
}

void XQLocationPrivate::requestUpdate()
{
}

void XQLocationPrivate::startUpdates(int msec)
{
    updateInterval = msec;
}

void XQLocationPrivate::startUpdates()
{
}

void XQLocationPrivate::stopUpdates()
{
}

XQLocation::Error XQLocationPrivate::openConnectionToPositioner()
{
    return XQLocation::NoError;
}

void XQLocationPrivate::closeConnectionToPositioner()
{
}

// End of file
