/****************************************************************************
**
** Trolltech hereby grants a license to use the Qt/Eclipse Integration
** plug-in (the software contained herein), in binary form, solely for the
** purpose of creating code to be used with Trolltech's Qt software.
**
** Qt Designer is licensed under the terms of the GNU General Public
** License versions 2.0 and 3.0 ("GPL License"). Trolltech offers users the
** right to use certain no GPL licensed software under the terms of its GPL
** Exception version 1.2 (http://trolltech.com/products/qt/gplexception).
**
** THIS SOFTWARE IS PROVIDED BY TROLLTECH AND ITS CONTRIBUTORS (IF ANY) "AS
** IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
** TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
** PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
** OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
** EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
** PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
** PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
** LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
** NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
** SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** Since we now have the GPL exception I think that the "special exception
** is no longer needed. The license text proposed above (other than the
** special exception portion of it) is the BSD license and we have added
** the BSD license as a permissible license under the exception.
**
****************************************************************************/

//#include <QtGui>
#include <QApplication>
#include <QPixmap>
#include <QSplashScreen>
#include <QWaitCondition>
#include "QDashWindow.h"
#include "ui.h"
#include "qtracker.h"
#include "geodata.h"

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

