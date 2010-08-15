//#include <QtGui>
#include <QApplication>
#include <QPixmap>
#include <QSplashScreen>
#include <QWaitCondition>
#include "qdashwindow.h"
#include "ui.h"
#include "qtracker.h"
#include "waypointlist.h"
#include "tracklist.h"

#ifdef Q_OS_SYMBIAN
#include <QSymbianEvent>
#include <w32std.h>
#endif

#include <QDebug>
#include <iostream>
//#define LOG( a ) std::cout << a << "\n"
#define LOG( a ) qDebug() << a
//#define LOG( a ) 

qTracker* qTracker::instance=0;
qTracker::qTracker(int argc, char* argv[])
: QApplication(argc,argv)
, settings("karpeer.net","qTracker",this)
{
	LOG( "qTracker::qTracker()"; )
	instance = this;
	//settings.clear();
	
	QPixmap pixmap(SPLASHRCDIR "splash.png");
	QSplashScreen splash(pixmap);
	splash.show();
	splash.showMessage("qTracker v" VER,Qt::AlignLeft);
	processEvents();
	dash = new QDashWindow(settings);
	dash->setAttribute(Qt::WA_DeleteOnClose);
	dash->Init(&splash);
	dash->show();
	splash.hide();
}
	
qTracker::~qTracker()
{
	LOG( "qTracker::~qTracker()"; )
	TrackList::Instance()->SaveSettings();
	WayPointList::Instance().SaveSettings();
	settings.sync();
}

#ifdef Q_OS_SYMBIAN
bool qTracker::symbianEventFilter (const QSymbianEvent * symbianEvent)
{
	const TWsEvent *event = symbianEvent->windowServerEvent();
	if (event)
	{
		switch( event->Type() ) {
			case EEventFocusGained: 
			{
				LOG( "qTracker::symbianEventFilter(): EEventFocusGained"; )
				break;
			}
			case EEventFocusLost: 
			{
				LOG( "qTracker::symbianEventFilter(): EEventFocusLost"; )
				break;
			}
			default:
				break;
		}
	}
	
	if ((symbianEvent->type() == QSymbianEvent::CommandEvent) && (symbianEvent->command() == EEikCmdExit))
	{
		LOG( "qTracker::symbianEventFilter(): EEikCmdExit"; )
		quit();
		return true;
	}
	
	return QApplication::symbianEventFilter(symbianEvent);
}
#endif

