<img src='http://qtracker.googlecode.com/svn/wiki/images/Scr000097.jpg' height='160' border='0' width='90' />
<img src='http://qtracker.googlecode.com/svn/wiki/images/Scr000096.jpg' height='160' border='0' width='90' />
<img src='http://qtracker.googlecode.com/svn/wiki/images/Scr000099.jpg' height='160' border='0' width='90' />
<img src='http://qtracker.googlecode.com/svn/wiki/images/Scr000085.jpg' height='160' border='0' width='90' />
<img src='http://qtracker.googlecode.com/svn/wiki/images/Scr000087.jpg' height='160' border='0' width='90' />
<img src='http://qtracker.googlecode.com/svn/wiki/images/Scr000092.jpg' height='160' border='0' width='90' />
<img src='http://qtracker.googlecode.com/svn/wiki/images/Scr000093.jpg' height='160' border='0' width='90' />
<img src='http://qtracker.googlecode.com/svn/wiki/images/Scr000095.jpg' height='160' border='0' width='90' /><br>
<hr />

<b>News:</b>

<ul><li>June 14th 2012</li></ul>

<blockquote>It's been a while without much news, i guess there are just so many apps for android that more or less do what i intended to do with qtracker that i don't have much need to pursue this anymore.</blockquote>

<blockquote>Still i've been looking to update this code, cause i really like the gauges, and i would like to have the multiple datum support back which was in the original qtracker.</blockquote>

<blockquote>So, these past few weeks, i've been working with necessitas again, worked out a new client-server api based on xml-rpc to make it easier to handle in qml, and have cleaned up the client and server, as well as the components, quite a bit. To make room for the new implementations i've cleaned up the trunk, and created a few new base directories.</blockquote>

<ul><li>August 27th 2011</li></ul>

<blockquote>Still busy getting things to run on Android, but now that the tripdataserver is running, and is able to start/stop trips & tracks, i can move towards improving the ui again.<br>
I've improved the map import page, and the trip & track selection & edit pages. There still is a bug somewhere when reading the trip time back, but that seems like a minor issue.<br>
What's next is import of GPX into category, and showing Waypoints & Tracks on the map. Also auto-map select might be a nice thing to have...<br>
However, the comming two weeks, there will be very little progress. I'm on a business trip from monday till friday, and on holiday the week after (finally get to try this app again in the mountains ;-)</blockquote>

<blockquote>Status: Most features are back to where they were prior to the ui redesign, but some things don't work yet (e.g. import GPX). A new Android version has been uploaded, and code submitted. Symbian build is currently broken, and needs some work to be on par with the android build.</blockquote>

<ul><li>August 14th 2011</li></ul>

<blockquote>After struggling with the QtMobility service framework and QLocalSocket, I have given up getting this to work on Android, and moved to QTcpSocket which i've now been successfully using to implement a new framework for client-server communication in Android. The bad news is: sockets don't work well on symbian, so I'll have to see if i will stick with the Publish & Subscribe interface there.</blockquote>

<blockquote>While moving over to using a new method of communication between client & server, i decided to rethink and restructure the daemon too, so rebuild it from the ground up. This means that the port to Android for the daemon is not completed yet, since i need to port some data-gathering functions over.</blockquote>

<blockquote>I did manage to get Map import function working again, and i've posted an Android apk (qtracker-client-0.4.423b.apk), which should be able to import maps that reside in the /mnt/sdcard/qtracker/maps directory.</blockquote>

<ul><li>August 14th 2011</li></ul>

<blockquote>My N97 has passed its 2nd aniversary last week, for me time to buy a new phone, and new contract. Since i can't see current Symbian phones having a bright future, and Meego not being released anywhere near here, i opted for a non-Nokia phone, for the first time in almost 5 years.</blockquote>

<blockquote>My new phone is a HTC Evo 3D, an Android Gingerbread based phone, which is up to par with the latest and greatest phones of this time, and that's how i like it ;-)</blockquote>

<blockquote>So, what about qTracker then? Well, over the last few days, i've worked on getting things to work using Necessitas & Ministro on my new phone. I've got a basic client working now, and put it up in the downloads section. I'm not able to load maps yet (need to sync up with my Symbian ^1 repository), but will probably fix that in the comming days.<br>
A bigger problem is the Publish&Subscribe interface, i use it to communicate between client and server, but it is missing in the Android version. I need to look for a different way to deal with this then.</blockquote>

<blockquote>Once i got the basic map loading & positioning working for Android, I'll focus on consolidating my Symbian & Android repositories. Hopefully i'll be able to clean up the portability issues w.r.t. Symbian^3, Maemo & Meego as well. There's no reason why this software won't run on all those platforms ;-)</blockquote>


<ul><li>August 2nd 2011</li></ul>

<blockquote>Checked in some minor updates, most importantly, track start should work now (although there seems to be a bug in naming it properly). Also fixed the display of floating point numbers in several places for speed/distance/altitude. I can't seem to figure out why Waypoints are showing up fine in the emulator, but not on my N97... this needs to work before i start working on displaying Tracks & Routes on the map.</blockquote>

<ul><li>July 4th 2011</li></ul>

<blockquote>Recently i've been making a lot of changes, causing functionality to break here and there. Most noteworthy, i'm moving all of the database code into c++, providing qml classes on top of that (TDatabase, TWaypoint, TRoute, TTrack, TTrip, TCategory, TMap & TRefpoint). Apart from that i've been restructuring the UI, now making more use of standard ListView, using model data retrieved from above mentioned classes. I've also made some changes in the database scheme, so you may want to remove your database prior to installing a new version so that it can re-initialise the database. Alternately you can use an sqlite editor to make changes yourself (see the top of qmldatabase.cpp for information on how it should look).</blockquote>

<blockquote>Right now, most functionality seems to work again in my workarea, so within a few days i'll hopefully have a reasonably working svn archive again. Untill then, expect things to break when using a version later than 413.</blockquote>

<ul><li>June 21st 2011</li></ul>

<blockquote>Uploaded new images for qtrackerd and qtracker-client, these solve most of the open issues. New map calibration code in place, which means that old style maps need to be imported into the database.</blockquote>

<blockquote>It seems my targets for the first alpha release are getting closer, just have to do another dialog box for adding calibration points, and create an on-screen display for the current wgs84 gps position.</blockquote>

<blockquote>Although this new release seems to work a lot smoother than the previous, i'm still running into segfaults every now and then so it is still unstable. I have been trying to look into this, but the debugger doesn't show me much of a stack trace. Just an "Cannot access memory at address 0x8028fcec" message indicating a segmentation fault. I guess i'm dereferencing an invalid pointer somewhere, but can't seem to track it down.</blockquote>


<ul><li>June 18th 2011</li></ul>

<blockquote>Ok, it's been over a month now, so i'll explain what i've been doing. The last weeks of may, and early june, the weather has been very nice here, so i've been working less on qTracker, but have been testing it out on my bicycle instead ;-).</blockquote>

<blockquote>There are some improvements on the way though, one is that i've implemented a 'trip' concept, which i'd like to use to associate data so that it can be more easily accessible.  A new trip is started each time qtrackerd is started, or upon request. An automatic name is generated, but it can be edited via the UI. I want the trip object to be a container for map/waypoint/route/track information, as well as keep some statistics (trip distance, time, min/max altitude, max speed, etc). This concept will also make it easier to export trip data as a group. Waypoints/routes/tracks can be easily exported as gpx, for map data and trip meta data, i can define an extension and still be gpx compatible.</blockquote>

<blockquote>I've got code to import/export gpx from/to the database in place, next step is to implement the UI, but this is not trivial, since i want to redo the layout to match the trip concept.</blockquote>

<blockquote>Last but not least, i am somewhat unhappy about the way my TableModel implementation works, so i'm considering to move more of this code into C++, so that on qml level only things like 'database.getWaypoints(tripid)' and 'database.getMaps(location,radius)' need to be available, which will return a list that can be used in a ListView item.</blockquote>

<blockquote>A new version will be uploaded, as soon as most of these changes are implemented in a sane way.</blockquote>

<ul><li>May 10th 2011</li></ul>

<blockquote>In a few minutes, a new binary release will be available in the downloads section, as of the final release version of QtSDK 1.1, i am able to build a working qtrackerd again, and qtracker-client seems to do well enough as well, albeit it does seem to crash more often than i like. The good news is that qtracker-client now keeps persistent settings for map and position, so when restarted it will restore the mapview as it was before the crash. Nevertheless, this does need attention.</blockquote>

<ul><li>April 25th 2011</li></ul>

<blockquote>It's been a while since my latest binary release, the major reason is that i've moved my development environment over to the new Qt SDK RC1, but am unable to build a working qtrackerd server application with it. The reason seems to be QtMobility 1.1.2.</blockquote>

<blockquote>My client application works fine against an older version of qtrackerd, since the interface hasn't changed, but getting them to work together requires a rather complex installation scenario since the client depends on QtMobility 1.1.2, and the server QtMobility 1.1.1.</blockquote>

<blockquote>Anyway, besides these problems, things are progressing well. I've restructured the codebase because there were simply to many qml files in a single directory, i've expanded my TableModel qml element so that i can use it to append and modify rows, as well as retrieve/set an entire row at once, and i've put a persistent storage class in place to store settings (Settings.qml).</blockquote>

<blockquote>Last but not least, the code for the gauges has been updated, the main gauge can now be swapped with any other gauge, and gauges can popup an (still empty) options screen when doubletapped.</blockquote>

<blockquote>Dialogs for adding waypoints and calibration points, creating routes and tracks, and editing them, are all in place. Functionaly they still need to be connected to the proper places. For Waypoint/Route/Track, i can simply add them to the database. As for Calibration data, i'm considering to move that into the database as well, but then i need to think about what is the most convenient way to deal with existing map calibration data.</blockquote>

<ul><li>April 3rd 2011</li></ul>

<blockquote>An updated client and server version have been uploaded, these will work together and actually do track recording (albeit the track can not be shown/exported yet, so it only resides in the sqlite database for now).<br>
I'm in the process of redesigning my menu/options screens, which is taking quite some time due to low level qml widgets being absent. (I tried using Colibri or Qt-Components, but both are simply too immature or not enough documented to be useful right now)</blockquote>


<ul><li>March 27th 2011</li></ul>

<blockquote>I've posted client-server sis files for qTracker, versions 0.3.314 and 0.3.319. The client is already showcasing the new widget layout, which is still a bit limited in use.<br>
Also the server part, apart from keeping the gps fix, currently doesn't add much value, but in the future it will take care of alarm monitoring & track recording.<br>
I would have committed the sources for 0.3.319 to svn, if only i could get subversion to accept the files... currently i am unable to check-in it seems...</blockquote>

<ul><li>March 23rd 2011</li></ul>

<table><thead><th><img src='http://qtracker.googlecode.com/svn/trunk/screenshots/qmltest.png' height='160' border='0' width='90' /></th><th>It's been rather quiet on the svn archive here, but that doesn't mean development hasn't progressed. My workarea is in desperate<br>need of clean-up, and therefore check-ins have stalled recently. As for what i've been up to so far, I've split the system into a<br>client-server framework, where the ui process kicks off a server process that does the GPS  and sensor monitoring, and which can<br>remain active after exiting the UI (thus saving on resources). Apart from that, i've been looking into the SQLite integration in<br>Qt, and am currently using that to do track recording (yes, that basic functionality is now also working ;-).<br><br>Last but not least, I'm also working on a new widget/layout set using qt-components, but am currently not able to run this<br>properly on my phone. So this will probably not be in the next release.<br><br>As soon as i get my workarea cleaned up, i will push out another qml based release, with most new features integrated.</th></thead><tbody></tbody></table>

<ul><li>March 3rd 2011</li></ul>

<blockquote>Uploaded v0.1.303, which was build against the recently released new beta SDK 1.1, with Qt 4.7.2 and QtMobility 1.1.1. As for new features, this one has a working satellite gauge, apart from that it is almost identical to the 300 build.</blockquote>

<ul><li>February 27th 2011</li></ul>

<blockquote>Uploaded a 0.1.300 version, which has tracking on map working fine, qmltracker is starting to become very usable now. Next milestone: Map calibration.</blockquote>

<ul><li>February 26th 2011</li></ul>

<blockquote>The problem in the tile expiration code is now fixed, and to improve performance, qmltracker now creates a tilecache for each map it loads. Once all tiles are cached, scrolling performance is similar to the non-tiled system. I've tested this with a 25Mpixel map, and it works fine. While the cache is not filled yet, scrolling may be slow though.</blockquote>

<ul><li>February 24th 2011</li></ul>

<blockquote>Got a tile-based mapviewer working now in qmltracker, not sure if i'll keep it, since it is initially quite slow because loading the tiles individually takes more time. It does show however that qmltracker is able to deal with large (up to 25Mpixel) images this way...</blockquote>

<blockquote>There does seem to be a problem still in the tile expiration code, somehow old tiles don't get expired when new ones are needed, thus requiring the map to be reloaded before other parts can be shown. So far, i've noticed this only in maps i was previously unable to load, so it shouldn't be a big problem ;-)</blockquote>

<ul><li>February 22nd 2011</li></ul>

<blockquote>For some time now, i've been wondering how i can get bigger images shown in my application, since with recent versions, i can't seem to load some of my bigger images anymore. It turns out that this is not due to lack of stack/heap memory in my application though, but most likely the font & bitmap server is running low on heap memory (this server handles bitmap data for all active applications).<br>
So far i've been unable to find a way to increase the heap size of the font & bitmap server, and i'm not sure if that is at all possible. This means that probably i'll have to find a different way to load images. There is more than enough RAM available (i notice i have about 33Mbytes free, which should be enough to load an 8Mpixel 32bit bitmap).</blockquote>

<ul><li>February 21st 2011</li></ul>

<blockquote>I'm having a lot of fun with qml, and so uploaded a recent version of qmltracker, this one can load a map, and convert map coordinates to wgs84 coordinates. Many improvements to the gauge models, but satellite view is not yet functioning.<br>
Auto positioning of the map is also not yet implemented, but is on top of my priority list ;-)</blockquote>

<ul><li>February 14th 2011</li></ul>

<blockquote>Having installed the new Qt SDK, i've been playing with the new QML Qt-Quick declarative language. This seems to make many things a lot easier. Just to see what it can do, i decided to start porting the qTracker UI over to QML. I was quite surprised to see that i got most functionality up and running after only a few nights hacking. The actual map positioning is not working yet, and neither are the option dialogs, but most of the rest is functioning, which is good enough to evaluate its performance.</blockquote>

<blockquote>It may well be that a next version of qTracker will be a QmlTracker ;-), but for now, i've uploaded a zip file with my project contents (running the SDK under windows 7 at the moment), and a resulting sis file for the N97. It uses a different ID from qtracker, so both can be installed in parallel.</blockquote>

<blockquote>Note that QmlTracker expects a (not included) map image in e:\data\QmlTracker\map.jpg</blockquote>


<ul><li>February 5th 2011</li></ul>

<blockquote>After playing with the new Qt SDK for two nights, i am now able to build working SIS files for Qt 4.7.1/QtMobility 1.1.0. I've already checked in most changes (build 270), and will upload a binary package soon. Build for maemo seems to work fine, but there may be some paths wrong. However, since i have no device to run it on, i really don't know if it will work.</blockquote>

<ul><li>February 3rd 2011</li></ul>

<blockquote>It looks like there's a new technology preview of the Qt SDK with Qt 4.7.1 and QtMobility 1.1.0, so it's time to play again ;-) Hope it won't be too hard to get things working for this platform.</blockquote>

<ul><li>December 19th 2010</li></ul>

<blockquote>Winter is here... lots of snow & ice... nice for an afternoon hike if it is sunny, for the rest, it is mostly too cold to go outside. Meanwhile several updates have been published to Qt, up to 4.7.1 now, and Qt Mobility 1.1.0 has been released.</blockquote>

<blockquote>I've been playing with the 4.7.1 release, but haven't managed to get things to work. I really wish that Nokia would release a Nokia Qt SDK and includes Qt Mobility 1.1.0, symbian support, and the mobile simulator... but the current Nokia Qt SDK is still at 4.6.x...</blockquote>

<ul><li>October 11th 2010</li></ul>

<blockquote>I haven't had much time to work on qtracker recently, and the time i did spend seems to have brought me little progress. I've tried to move over to qt 4.7.0, but so far have not been able to build a working version using this new release. I guess this is not as simple as i had hoped it would be.</blockquote>

<blockquote>Winter is comming, and last weekends good weather may very well be the last we'll see for some time. Usually this means i am less motivated to work on qtracker for a few months, until spring comes. Perhaps when a new Nokia Qt SDK appears, I'll see if i can get things to work with it. Apart from that, development will be slow.</blockquote>


<ul><li>September 26th 2010</li></ul>

<blockquote>I spent some time on a different hobby of mine, mythtv (www.mythtv.org) trying to get it to work with my local DVB-T pay-tv provider. So progress on qtracker is a bit slow right now. I did notice recently Qt 4.7 was released, so i plan to get things working against the official Qt 4.7 release this comming week. Also i still need to fix the GPX import, which is almost ready. This means that even though there is not much new functionality, i still want to get a new binary out this week.</blockquote>

<blockquote>Recently, i got some feedback that qtracker also runs on the N900, although there are still issues with the paths & images. This means that i will also give some attention to the Maemo build, clean up the code to use platform specific paths for the images, tracks & routes. If things work out, i will put a Maemo binary package up on the site as well.</blockquote>

<ul><li>September 14th 2010</li></ul>

<blockquote>Since my last update, i have little news. My virusscanner detected a problem on my drive, and while trying to fix it, ruined the installation. This means i've been busy setting up a new environment (under Windows 7 this time), and although it seems to be up and running again, i've not had time to do much else yet.</blockquote>

<blockquote>I tried to take the opportunity and see if i can switch from Carbide to QtCreator, but alas, i am still not able to build a working qTracker build for symbian using QtCreator...<br>
So I'm back to Carbide again.</blockquote>

<ul><li>September 2nd 2010</li></ul>

<blockquote>I reworked the datum code, and implemented a generic dialog to handle UTM projections with translation, rotation & scale offsets.</blockquote>

<blockquote>The datum code also reads additional datums from that can be defined in the config file, this means that datums currently not supported by the code (e.g. sterea projections like dutch RD grid), can be added through the config file.</blockquote>

<blockquote>Also put the dialogs in place for import/export of GPX files, but they don't seem to work well in the emulator, and thus it is hard for me to work with this. On the device they seem to do ok, so i may continue on this path.</blockquote>

<ul><li>August 30th 2010</li></ul>

<blockquote>Since i couldn't get GeographicLib to do RD conversions, i decided to give Proj4 another try, and started with a limited projection set (latlong, utm, sterea, stere). After an evening of fiddling, i finally got it to do what i want, and i get the same results as on my Linux host, where i got the full package installed.</blockquote>

<blockquote>Now I'm reworking the datum handling code, to work with Proj4. The biggest issue seems to be how to get proper string representations converted from/to the projected datums (to be used in dialogs).</blockquote>

<blockquote>This also means that if your favourite datum/projection is currently not supported, it could be fairly easy to add it provided you got a working Proj4 defenition (see <a href='http://trac.osgeo.org/proj/'>http://trac.osgeo.org/proj/</a> and <a href='http://www.spatialreference.org/ref/'>http://www.spatialreference.org/ref/</a> ) and of course it would help if you can provide some test data for me to verify that things work properly.</blockquote>

<ul><li>August 28th 2010</li></ul>

<blockquote>Uploaded version 0.8.262, this has the clock options fixed, which accidentally got checked in disabled in build 254. Now they should work properly again.</blockquote>

<blockquote>Reworked the buttons in the mapview, now they are seperate widgets in the dashview, based on QToolButton. This cleans up a lot of code, and they now show some feedback when pressed.</blockquote>

<blockquote>Meanwhile i also installed Qt 4.7.0rc1. qTracker build 262 is build against this version. It may work fine with beta2, but i have not tried. Build 260 was built against beta2, but seems to run fine on rc1 as well.</blockquote>

<blockquote>I also discovered a new bug, it seems that (at least under some condition) selecting a route for display can crash qTracker.</blockquote>

<ul><li>August 27th 2010</li></ul>

<blockquote>I've uploaded build 0.8.260. This version has a new 'route' button on the top left next to the 'track' button, a new 'exit' button in the bottom right corner, and moved up the zoom-out button to the top right, underneath the zoom-in button.</blockquote>

<blockquote>I have been trying to get the RD coordinate transformation in, but didn't manage to get it to work, then realised it is not using transverse mercator, but stereographic projection. Not sure if geographiclib supports that. Anyway, that datum is not in yet.</blockquote>

<ul><li>August 23rd 2010</li></ul>

<blockquote>I've uploaded a new build, version 0.8.250. With this version other representations of the current position are possible. I've got support for Wgs84 (previous default), Wgs84 with DMS notation, UTM and MGRS in place. I'm looking into how to get RD mapped using GeographicLib. It's really not my thing, these datum conversions, so if anyone wants their favorite conversion/projection supported, then send me a bit of example code which shows how to convert it from/to wgs84 with GeographicLib, and I'll see if i can get it in qTracker.</blockquote>

<ul><li>August 21st 2010</li></ul>

<blockquote>Had a short holiday in the German Eifel, so not much happened the last week. But I'm back now, and looking into several matters.</blockquote>

<blockquote>First, there is still no support for other datums than Wgs84. I've made a start at integrating geographiclib ( <a href='http://geographiclib.sf.net'>http://geographiclib.sf.net</a> ) into qtracker, in order to support UTM, UPS and  MGRS, but the configuration dialog is not in yet.</blockquote>

<blockquote>Second, there seems to be a bug preventing the maps dialog to come up in some conditions, i hope to have that solved now, but need to verify.</blockquote>

<blockquote>Third, map scrolling is not very smooth recently, and so i've looked at the Direct Screen Access API. This is not a trivial matter to get working though... and will probably become an item for a later development cycle.</blockquote>

<ul><li>August 10th 2010</li></ul>

<blockquote>Uploaded version 0.8.226.</blockquote>

<blockquote>Added clock widget options, now you can select a function (current, trip, remaining, eta) for each of the clock items (analog, top bar, bottom bar). Also the monitor tab is there, so you can select a waypoint/route/track to monitor (currently only waypoint supported).</blockquote>

<blockquote>Note: I ran into some problems restarting qTracker after an upgrade. It took me an hour of puzzling before i thought of removing the persistent settings file and try again. That made the app work again, so i put up a special note about this in the installation guide.<br>
(i do remember changing the format of the starttime, that could have caused the problem).</blockquote>

<ul><li>August 8th 2010</li></ul>

<blockquote>Updated the way data is passed to the widgets, only a single positioningserver and compass sensor is used now. Also combined the ETA, Trip time and Local time into a single clock widget. A signal strength widget replaces the trip time widget, and shows compass calibration level, battery strength and number of satellites used. I'll probably drop the last one in favour of either GSM signal strenght, or horizontal/vertical accuracy.</blockquote>

<blockquote>Reworked the compass options dialog, now the monitor has a seperate tab. Intention is to let the tab appear in any widget which is capable of showing some monitored property (compass, speed/distance, clock).</blockquote>

<blockquote>Update: By coincedence, i discovered that ReadDeviceData capability doesn't seem required for compass to work properly... so i'm free to use selfsigned, even with the compass enabled. I've uploaded a 0.8.214 selfsigned package which works fine on my N97 with compass enabled.</blockquote>

<ul><li>August 1st 2010</li></ul>

<blockquote>Fixed compass update frequency, default setting was unreliable, causing the compass to not work at all sometimes.<br>
Implemented route display, place your (.gpx) routes in !:/Data/qTracker/Routes and they will automatically appear in the routes list. For now the route list is under the button on the left bottom of the map.</blockquote>

<blockquote>Since I'm using the compass sensor now, qtracker needs ReadDeviceData permissions, which means i can't release a selfsigned version anymore, since that won't have enough permissions on the device. Therefore you need to go to www.symbiansigned.com and use opensigned to sign it for your own phone. Perhaps later I'll put downloads online with the Compass disabled, which do not need to be signed, but it has little priority for me now.</blockquote>

<blockquote>I've started to remodel the way data is comming into the app, now each place that needs a position is doing its own instantiation of a positioning server etc. I've setup a datamonitor class which will do all this centrally, so all users can simply connect to events comming from there. Still work in progress though.</blockquote>

<blockquote>All this is in version 0.8.206, which I've put in the downloads section.</blockquote>

<ul><li>July 29th 2010</li></ul>

<blockquote>Fixed the compass settings dialog, and got the compass sensor to work with my N97 as well. Currently the application uses all capabilities grantable by opensigned.symbian.com, i have yet to figure out which i really need for the compass. Once i do, i'll strip the capabilities down again.<br>
Todo: Find a way to display compass calibration level.</blockquote>

<ul><li>July 28th 2010</li></ul>

<blockquote>Got the compass settings dialog in place, and some of them working: You can now choose between 'north up' and 'heading up' for the compass, and you can choose a waypoint to monitor. The waypoint direction will be shown using the new outer ring on the compass (red dot towards waypoint).<br>
Somehow the settings are not taken over immediately after leaving the dialog, I will look into that matter, and upload a new version when i got it fixed.</blockquote>

<ul><li>July 25th 2010</li></ul>

<blockquote>I've prepared a new version (192), and did some preliminary testing on it. Most functionality seems to work properly, also on my phone. I did see one strange thing, a crash when failing to load a map under low memory conditions. I'ts something to look into, i want qTracker to fail 'gracefully' if it can not load a map.</blockquote>

<ul><li>July 24th 2010</li></ul>

<blockquote>I've moved the 'add refpoint' dialog into the map dialog as a seperate tab next to the local & all tabs. It's now possible to add refpoints, even after the map is properly calibrated (although the code doesn't use them yet). I also want to put an editable refpoint list in place, but it has no priority  just yet. Also implemented an auto-save for the map meta data on every added refpoint. This means that map calibration should work properly now.<br>
Will have to do a little more testing, i'll want to check the map calibration code in real life, and add/remove a few refpoints, waypoints & tracks. If all works well, i'll have a decent release within a week or so, which has all the basic features in place (map display & calibration, waypoint & track recording).</blockquote>

<ul><li>July 22nd 2010</li></ul>

<blockquote>Implemented new waypoint dialog. Currently refpoint adding is not enabled, when that is done, i'll upload a new version which hopefully also fixes a bug in the adding of refpoints (bug was introduced in changeset 114, so 0.8.111 should still work fine).</blockquote>

<blockquote>Meanwhile also implemented persistent settings using QSettings. Most settings for the dashview and mapwidget are now persistent. Waypoints still to be done...</blockquote>

<blockquote>You may have noted that it is now impossible to close qTracker when it is recording a track. I've put the application into System Application mode when it is recording. This will prevent the red-button exit, and also prevent low-memory conditions to exit the application. If you do want to exit, just stop the track, and then press the red key.</blockquote>

<ul><li>July 21st 2010</li></ul>

<blockquote>Got track show/hide/delete functions working, tracks are recorded in gpx format in !:\Data\qTracker\Tracks, and are now listed on the 'list' tab of the track dialog box.<br>
The list shows a 'x' symbol which can be pressed to delete the track, and a 'o' or 'v' symbol to denote weter the track is shown ('v') or not ('o'). When pressed, the symbol will toggle between the two, and the map will be updated accordingly.<br>
I'm thinking to add a filter, so you can limit the list to 'visible', 'recent', 'onmap' or 'all' tracks.</blockquote>

<blockquote>Note that when deleting tracks, some weird effects happen during the update of the widget. Cancel the dialog, and bring it up again, and the list is shown properly again.</blockquote>

<blockquote>Edit: Just uploaded version 0.8.184, which fixes the track recording crash found in 0.8.149</blockquote>

<ul><li>July 20th 2010</li></ul>

<blockquote>Updated my environment to Qt 4.7.0-beta2, experimented some with the new Simulator, and<br>
updated the stylesheet for the dialogs in the mean time because it looked awfull with<br>
the simulator.<br>
Added the symbols for delete & show/hide track in the tracklist tab. All that's left<br>
now is to connect the signals to delete/show/hide actions in the map widget.</blockquote>

<ul><li>July 16th 2010</li></ul>

<blockquote>Got the 'new' track dialog tab functioning properly now, you can select either<br>
to record all points, use a time based threshold, or use a distance based threshold.<br>
Found some rarities in QDateTime (using Qt 4.7 beta1), it doesn't seem to properly<br>
convert to string, and also the secsTo() function doesn't behave as i would expect...<br>
<a href='https://code.google.com/p/qtracker/source/detail?r=132'>Revision 132</a> has all the changes, i've uploaded a new package.<br>
(temporary removed the magnetometer stuff, it doesn't work, and with this disabled i can use self signed packages ;-) )</blockquote>

<ul><li>July 14th 2010</li></ul>

<blockquote>Added a track symbol, will bring up a track recording dialog, or save and stop recording if recording already.<br>
The flag symbol now is back as refpoint/waypoint dialog.<br>
Still need to implement waypoint save/restore and track loading.</blockquote>

<ul><li>July 13th 2010</li></ul>

<blockquote>Did some major redesign with respect to the map meta data handling, so things may be a bit unstable as a result.<br>
Got a basic structure in place now to deal with tracks, routes, waypoints and maps.<br>
Got the basics for a GPX file reader in place, writer still to be done.<br>
Got waypoint and track recording basics working.<br>
Todo: Create a proper UI to deal with Tracks/Routes/Waypoints, the single flag won't do it anymore (currently mapped to track recording).</blockquote>

<ul><li>July 2nd 2010</li></ul>

<blockquote>QCompass/QCompassReading... I've been trying to get these to work properly, since my<br>
N97 has a built-in magnetometer sensor. Code is in place, but so far i just don't seem<br>
to get any information from the sensor... I guess this requires some more debugging.<br>
<br>
<br>
<BR><br>
<br>
Edit: Perhaps it has to do with this <a href='http://bugreports.qt.nokia.com/browse/QTMOBILITY-235?page=com.atlassian.jira.plugin.system.issuetabpanels:changehistory-tabpanel'>bugreport</a></blockquote>

<ul><li>July 1st 2010</li></ul>

<blockquote>Uploaded a self-signed package based on release 111 (v0.8.111).<br>
With this version, I've dropped my own modified 'branch' of the QtMobility classes in<br>
favor of the QtMobility 1.0.0 API (which now does have all the features I require).<br>
This version also works on Qt 4.6.3, but somehow building against QtMobility 1.0.1<br>
seems to break the app (prevent it from starting), so for now I'm sticking to 1.0.0.</blockquote>


OldNews