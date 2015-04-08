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

<h1>New qTracker wishlist:</h1>

<h3>Handle OziExplorer calibration data</h3>
OziExplorer is quite a popular program used to prepare maps and plan trips using a similar method to qTracker, but aimed at use for PC & PocketPC (the fact it was lacking a Symbian version is what inspired me to start with qTracker in the first place). A lot of existing maps have OziExplorer compatible calibration data, and thus it would be convenient if qTracker can use these as well.<br>
<br>
<h3>Use QtMobility Map Viewer API</h3>
This would be a major change, but moving over to this API could be another approach to solving the problem of handling tiled based maps in a generic way.<br>
<br>
<h3>Use QImageReader to read map data (possibly as tiles)</h3>
There seems to be a QImageReader class, which might improve performance if the underlying io handler supports clipped/scaled jpeg image reads.<br>
If supported, then this could enable the use of 'huge' jpeg based maps without the need of splitting the image itself into seperate files.<br>
<br><br><b>Note:</b> This is already implemented in qmltracker, the qml based test version of qTracker. For performance reasons, the resulting tiles are cached, since partial reads of a large image file are very slow.<br>
<br>
<h3>Use tiled (Geo)Tiff files</h3>
A big limitation of qtracker is that it uses jpg images that need to be completely loaded in memory. The advantage is that a camera image can immediately be used as a map, by calibrating 2 points, however, a big disadvantage is that it limits the size of the map that can be used.<br>
By implementing a camera engine that can save the max resolution to a tiled Tiff file, this problem can be overcome, since qtracker simply loads the tiles needed.<br>
This also has the advantage that external tools can be used to scan large maps, and keep the data together into a single map file. GeoTiff even has the added advantage that datum & calibration data can be stored with the image.<br>
<br>
<br>
<h3>Show (google earth) tracks on the map</h3>
What I miss most of all from tracker-py is the option to show google earth tracks on<br>
the maps.<br>
<br>
<ul><li>this is now possible, provided you convert them to gpx format, and place the files in the :/data/qtracker/tracks directory.</li></ul>

<br><br>
<hr />

<h1>From tracker-py wishlist:</h1>

<h3>Non-cylindrical (Lambert, others?) projection support</h3>
A lot of topographic maps use Lambert conic projection, but the current<br>
version of tracker-py doesn't know any projection but cylindrical one.<br>
<br>
<h3>Better map calibration</h3>
Tracker can use only two calibration points at the moment, and<br>
the current algorithm assumes that the maps are aligned with the<br>
WGS84 north.<br>
A better algorithm should be possible, using more of the calibration<br>
points, and taking rotation into account.<br>
<br>
<h3>Split UI & Data processes</h3>
Currently the UI and the Data model are designed as seperate<br>
components, yet they run together in a single process.<br>
It would be nice to be able to run the dataprovider/datastorage<br>
parts of Tracker in seperation as a daemon.<br>
That way, the resource hungry Mapview ui does not always need to<br>
be running while the positioning part is running (to record a track<br>
or monitor a waypoint).<br>
On certain events, a ui process can be started automatically.<br>
<br>
<h3>Exchange of Landmarks</h3>
A message format could be defined to send/receive Landmarks over SMS/E-Mail/Bluetooth/Other?<br>
When a landmark comes in the user can for instance choose to Display on Map or Save to<br>
Landmarks. Similarly it would be nice if a Landmark, Current Position, or Selected Position<br>
on the map can be send to others. Perhaps an automatic action could also be defined upon<br>
reception of a Landmark. Feature could also be extended to routes/tracks (although<br>
tracks could contain large amounts of data)<br>
Suggested message formats: NMEA, GPX<br>
<br>
<h3>Localization</h3>
Currently Tracker.py is only available in English. It would be nice if all<br>
UI messages and texts were localized so that other languages can be easily<br>
added.<br>
<br>
<h3>Auto-save on Low-power</h3>
During a recent cycling trip, I found that I lost the entire track which<br>
I was recording, simply because the battery ran out. Now tracker already<br>
monitors the battery status, it would be a good idea to do an autosave when<br>
the battery charge drops below a treshold of say 5% or 10%.<br>
<br>
<h3>Low-power Tracking</h3>
When the GPS, mapview & gauges are active (even with dimmed display) the<br>
phone consumes a lot of power.<br>
So for trips longer than a few days you may not have enough power...<br>
Now I could buy some more batteries, but perhaps it is also possible to<br>
implement a sort of 'interval' tracking mode, having tracker only wake up<br>
once every 10 minutes or so, wait for a fix, store the trackpoint, and go<br>
to sleep again for 10 minutes. When hiking (especially in the mountains),<br>
10 minutes is not a long time, and the track will still be pretty accurate.<br>
I'm not sure how much power would be saved this way, but surely it must make<br>
a significant difference.<br>
Another way to figure out when to power-up the GPS could be to use the cell<br>
signal strength/number of different towers passed, to detect movement/<br>
estimate distance (although this might not help much in mountain areas where the gsm reception is often absent...).<br>
<br>
<h3>Advanced use of Landmarks and Categories</h3>
Right now, tracker always shows all landmarks in the waypoint category,<br>
thus depending on an other editor to be used for moving waypoints in/out<br>
of this category.<br>
It would be nice if tracker can create categories, and show a dialog where<br>
can be selected which categories/landmarks should be shown.<br>
One might want to use a new category per map, or per area.<br>
<br>
<h3>Direction-up map display</h3>
Map-display is done in North-up display, which feels right if you are used<br>
to reading paper maps.<br>
However, for many people it feels more natural to have a direction-up<br>
display, thus rotating the map whenever you change direction.<br>
<br>
<h3>Route or Track based calibration</h3>
Display a route or track on an uncalibrated map, and drag certain points<br>
of the route/track to a point on the map, thus calibrating the map.<br>
Ideas on how to tackle this:<br>
<ul><li>Detect sharp corners, and enable dragging these while updating map & track display<br>
</li><li>Rotate & scale the track/route, also with continuous screen updates.</li></ul>

<h3>Fetch maps from Google/Yahoo/MSN through their API</h3>
Google, Yahoo and Microsoft provide API's to access geographical/map data.<br>
Tracker could use these to download a map over the network.<br>
<br>
<h3>Use accelerometer in low-power mode</h3>
When GPS is powered off, use the accelerometer to detect strong movements,<br>
and thus as an aid to determine when to power-up the GPS to get a position.<br>
The accelerometer can also be used to powerdown the screen/gps, e.g. if the<br>
device is upside down.<br>
<br>
<h3>Monitor route/track</h3>
Currently it is only possible to monitor a waypoint. Similarly, it should<br>
be possible to monitor a route (as series of waypoints), or track.<br>
When in monitor track/route mode, an algorithm can be used to determine<br>
at which moments the GPS should be powered up/down, depending on the<br>
form of the current speed and distance to next corner/route point.<br>
<br>
<h3>Run external script when specified location is reached</h3>
Current waypoint monitoring code only allows for an alarm to be set upon<br>
reaching a specified waypoint within a set tolerance distance.<br>
It would be nice if an external event could be triggered when a certain<br>
location is reached, and it would be especially nice if a user-defined<br>
python script could be executed so that anyone can customize the actions<br>
according to their wishes.<br>
<br>
<h3>Others? Please let me know!</h3>
If something is holding you back from using this software, please let me<br>
know what it is. I can add it to this wishlist, and who knows maybe it will<br>
end up on the RoadMap as well.<br>
(contact me at mark.hurenkamp at xs4all.nl or post on the general discussion<br>
list)