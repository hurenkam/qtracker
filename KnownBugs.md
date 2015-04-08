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


<h3>Release 0.8.226</h3>
Scrolling has improved, but still is not as good as i would like.<br>
<br>
<h3>Release 0.8.214</h3>
<b>New</b>
<ul><li>Map scrolling is not very smooth anymore... perhaps widgets are updating themselves too often now, have to look into this, perhaps disable/lower frequency of updates while scrolling (<a href='https://code.google.com/p/qtracker/issues/detail?id=2'>Issue 2</a>).</li></ul>

<h3>Release 0.8.206</h3>
<b>Solved</b>
<ul><li>Compass problems (<a href='https://code.google.com/p/qtracker/issues/detail?id=5'>Issue 5</a>)</li></ul>

<h3>Release 0.8.200</h3>
<b>New</b>
<ul><li>Unreliable compass, strangely, it works fine if another compass application is running. Turns out it has to do with the datarate, which i did not set. Newer versions have it explicitly set at 1 per second, which seems to fix the problem (<a href='https://code.google.com/p/qtracker/issues/detail?id=5'>Issue 5</a>).</li></ul>

<b>Solved</b>
<ul><li>Refpoint problem (<a href='https://code.google.com/p/qtracker/issues/detail?id=4'>Issue 4</a>)</li></ul>

<h3>Release 0.8.184</h3>
<b>New</b>
<ul><li>Found what looks like a bug in the set refpoint code, it is possible that calibration of new maps doesn't work. Existing maps (e.g. from tracker-py) should work fine. I've fixed the problem in svn, will do some tests before uploading the new version (<a href='https://code.google.com/p/qtracker/issues/detail?id=4'>Issue 4</a>).<br>
</li><li>When deleting a track, the tracklist is not always properly updated, parts of the previous line may remain (<a href='https://code.google.com/p/qtracker/issues/detail?id=1'>Issue 1</a>).<br>
</li><li>Because i now use QXmlStreamReader to read all Xml based config files, some historic metadata files from tracker-py with the 'missing =' bug won't read properly, causing qTracker to hang during startup. Fixing the metadata by putting the '=' sign after the version attribute in the xml tag on the first line of the file solves this. I'll look into a generic solution to fix the hangup, since even bad data should not cause the app to stall (<a href='https://code.google.com/p/qtracker/issues/detail?id=3'>Issue 3</a>).