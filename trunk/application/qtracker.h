#ifndef QTRACKER_H
#define QTRACKER_H

#include <QApplication>
#include <QPixmap>
#include <QSplashScreen>
#include <QWaitCondition>
#include "QDashWindow.h"

#ifdef Q_OS_SYMBIAN
#include <QSymbianEvent>
#include <w32std.h>
#endif

class qTracker: public QApplication
{
private:
	QDashWindow* dash;
	QSettings settings;
	
public:
	static qTracker* instance;
	
public:
	qTracker(int argc, char* argv[]);
	~qTracker();

    #ifdef Q_OS_SYMBIAN
	virtual bool symbianEventFilter (const QSymbianEvent * symbianEvent);
    #endif
};

#endif // QTRACKER_H
