[About](http://code.google.com/p/qtracker/wiki/qTracker) |
[User Manual](http://code.google.com/p/qtracker/wiki/UserManual) |
[Installation Manual](http://code.google.com/p/qtracker/wiki/InstallManual) |
WishList | KnownBugs | RoadMap | QmlTracker

---

<img src='http://qtracker.googlecode.com/svn/wiki/images/Scr000041.jpg' height='90' border='0' width='160' />
<img src='http://qtracker.googlecode.com/svn/wiki/images/Scr000048.jpg' height='90' border='0' width='160' />
<img src='http://qtracker.googlecode.com/svn/wiki/images/Scr000053.jpg' height='90' border='0' width='160' />
<img src='http://qtracker.googlecode.com/svn/wiki/images/Scr000054.jpg' height='90' border='0' width='160' /><br>
<hr />

<h1>Introduction</h1>

qTracker is a port of tracker-py, to Qt/S60 aimed at use with the new Nokia N97.<br>
Since there are a lot of changes in that transition, a short user manual is probably<br>
in order...<br>
<br>
<h1>Using qTracker</h1>

First, there is no menu (yet), this is because up to now, i've been able to manage<br>
all functionality using dedicated widgets on the screen.<br>
<br>
<h2>Gauges</h2>
<img src='http://qtracker.googlecode.com/svn/wiki/images/Scr000041.jpg' height='180' border='0' width='320' />

qTracker by default starts out with 6 minimized gauges, and a map widget that<br>
occupies about half the screen. There are 4 types of event supported by the widgets<br>
in qTracker: SingleTap, DoubleTap, LongTap, and Drag. The gauges support the first<br>
three. There are currently no Gauges which use the Drag event type.<br>
<br>
SingleTap can be used to maximize a gauge, or to bring back the map, if the maximized<br>
gauge is SingleTapped. DoubleTap is to bring up the Gauge Options (not yet implemented<br>
in v0.0.6), and LongTap is meant to reset any data inside the gauge (reported min/max<br>
values for altitude, reported trip distance in speed gauge, and trip time in the trip gauge).<br>
Due to a bug, the altitude gauge is reset via DoubleTap, and not LongTap, this will be<br>
solved in a future release.<br>
<br>
<h2>Maps</h2>

<img src='http://qtracker.googlecode.com/svn/wiki/images/Scr000038.jpg' height='180' border='0' width='320' />
<img src='http://qtracker.googlecode.com/svn/wiki/images/Scr000048.jpg' height='180' border='0' width='320' />

A map can be loaded by pressing the button in the bottom left of the map widget, this<br>
will bring up a list of available maps in (!:/data/qtracker/maps), maps of type .xml and<br>
.jpg will be shown. When a .jpg file has a matching .xml file, it is assumed they belong<br>
together, and that the .xml file contains the metadata to be used with the .jpg file.<br>
When a .jpg file is loaded, the metadata will be created as soon as a reference point is<br>
defined (a reference point can be added by tapping the flag symbol on the topleft of the<br>
widget).<br>
The + and - signs on the right speak for themselves, they are used to zoom in/out with<br>
a range of 3 - 0.3.<br>
SingleTap on the map widget will cause qTracker to position the map at the current location,<br>
or, if the current location is not on the map, it will try to find and load a map that does<br>
contain the current location.<br>
DoubleTap on the map widget, brings up a list of maps for the current location.<br>
<br>
You can drag the map around, then it will go into scroll mode, and no longer display the<br>
current location. When an uncalibrated map is loaded, you can drag it to a point of your<br>
choice, then choose the topleft flag widget, the dialog is already filled with the current<br>
GPS position if available, and when you press confirm, it will add a reference point matching<br>
the map x,y position to the longitude/latitude given in the dialog. Two such refpoints will<br>
cause the map to autocalibrate, and from then on you can use the map to navigate.<br>
<br>
LongTap on the map will cause it to be maximized. LongTap it again to go back to default<br>
size. SingleTap any gauge, and the gauge will maximize, and the map will disappear.<br>
SingleTap the maximized gauge, and the map will come back.<br>
<br>
<br>
<h2>Tracks</h2>
<img src='http://qtracker.googlecode.com/svn/wiki/images/Scr000053.jpg' height='180' border='0' width='320' />
<img src='http://qtracker.googlecode.com/svn/wiki/images/Scr000054.jpg' height='180' border='0' width='320' /><br>

A track can be recorded by pressing the hiker logo (2nd item on the topleft of the screen), it will bringup the track dialog box, with as default the tab 'new' highlighted.<br>
You can fill in a name, and set a time or distance based interval. Press the Start button to start recording. The tab will change name to 'Current', and the start button is replaced with a stop, and an update button.<br>
You can now change the settings (e.g. change time/distance), and press update to make the changes active. You can press cancel to go back to the map, recording will still continue.<br>
Press the hiker button again, now the 'Current' tab comes back up again, with the settings as previously. Press stop to stop recording, the track is then saved to !:\Data\qTracker\Tracks as a gpx file.<br>
Use the 'List' tab to show/hide/delete the tracks. Press the red 'x' to delete a track, press 'o' to show (load) a track, or 'v' to hide (unload) a track.