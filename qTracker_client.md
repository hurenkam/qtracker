[About](http://code.google.com/p/qtracker/wiki/About) |
[User Manual](http://code.google.com/p/qtracker/wiki/UserManual) |
[Installation Manual](http://code.google.com/p/qtracker/wiki/InstallManual) |
WishList | KnownBugs | RoadMap |
[qTracker Client](http://code.google.com/p/qtracker/wiki/qTracker_client) |
[qTracker Server](http://code.google.com/p/qtracker/wiki/qTracker_server)

---

<img src='http://qtracker.googlecode.com/svn/wiki/images/Scr000097.jpg' height='160' border='0' width='90' />
<img src='http://qtracker.googlecode.com/svn/wiki/images/Scr000096.jpg' height='160' border='0' width='90' />
<img src='http://qtracker.googlecode.com/svn/wiki/images/Scr000099.jpg' height='160' border='0' width='90' />
<img src='http://qtracker.googlecode.com/svn/wiki/images/Scr000085.jpg' height='160' border='0' width='90' />
<img src='http://qtracker.googlecode.com/svn/wiki/images/Scr000087.jpg' height='160' border='0' width='90' />
<img src='http://qtracker.googlecode.com/svn/wiki/images/Scr000092.jpg' height='160' border='0' width='90' />
<img src='http://qtracker.googlecode.com/svn/wiki/images/Scr000093.jpg' height='160' border='0' width='90' />
<img src='http://qtracker.googlecode.com/svn/wiki/images/Scr000094.jpg' height='160' border='0' width='90' />
<img src='http://qtracker.googlecode.com/svn/wiki/images/Scr000095.jpg' height='160' border='0' width='90' /><br>
<hr />
<h1>qTracker v0.3 & v0.4 series (unstable)</h1>

This is essentially a port of qTracker to qml, split in a client and server part so that the client (which consumes a lot of resources) does not need to run all the time, because the server keeps track of trip and track recording data.<br>
<br>
Although the new qTracker client and server have not yet reached the maturity of the original qTracker, it is not likely i will continue on the original code base, because the new client/server version is in many ways already much more advanced than qtracker-0.8.270<br>
<br>
<h1>Requirements</h1>

Platform: Symbian<sup>^1</sup>, Symbian<sup>^3</sup> or Maemo<br>
<br>
<BR><br>
<br>
<br>
Qt versions: v4.7.x <br>
<br>
<BR><br>
<br>
<br>
QtMobility: 1.1.x <br>
<br>
<BR><br>
<br>
<br>
<br>
<h1>Features</h1>

<b>Works:</b>
<ul><li>Gauges:<br>
<ul><li>Compass:   North up or Heading up, use sensor or gps<br>
</li><li>Clock:     Current time, Trip time<br>
</li><li>Satelites: In view & In use<br>
</li><li>Speed:     Actual, Average, Min, Max, Trip Distance<br>
</li><li>Altitude:  Actual, Average, Min, Max, Trip Ascent, Trip Descent<br>
</li><li>Levels:    Battery, Compass sensor, Gps Accuracy<br>
</li></ul></li><li>Map zoom<br>
</li><li>Map centers on & follows gps position<br>
</li><li>Installation works on C: and E: (F: is untested, but should work)<br>
</li><li>Runs on Simulator & Symbian^1 (tested on Windows7/Ubuntu Maverick & N97)<br>
</li><li>Builds fine for Maemo & Symbian^3 (untested since i don't have those devices)<br>
</li><li>Record Tracks<br>
</li><li>Create & Edit Waypoints<br>
</li><li>Create & Edit routes<br>
</li><li>Persistent settings (current map, zoom mode, many others)<br>
</li><li>Client server model</li></ul>

<b>Todo before 1st alpha:</b>
<ul><li>Show wgs84 coordinates on map<br>
</li><li>Calibrate map</li></ul>

<b>Todo before 1st beta:</b>
<ul><li>Fix client stability issues<br>
</li><li>Show/Hide/Delete Trips<br>
</li><li>Show/Hide/Delete Tracks<br>
</li><li>Show/Hide/Delete Waypoints<br>
</li><li>Show/Hide/Delete Routes</li></ul>

<b>Other todo</b>
<ul><li>UTM datum support<br>
</li><li>Datum projection support for proj4 types: stereo, latlon, utm, tmerc<br>
</li><li>Map autoselect<br>
</li><li>Gpx import/export