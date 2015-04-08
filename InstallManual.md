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

<h3>qTracker client & server (0.3.xxx series)</h3>

Note that to properly work with this version, you need to install two sis packages, the qtracker-client-0.3.xxx.sis and the qtrackerd-0.3.xxx package. They may be a different version, but try to get the latest of both, because incidentally the IPC API may change.<br>
I've seen several downloads of qtracker-client, with the matching qtrackerd download count remaining 0... this means some people are not aware of this dependency... please beware!<br>
<br>
This does not apply for the 0.8.xxx series of qtracker.<br>
<br>
<h3>Dependencies</h3>
As of version 0.8.200, qtracker depends on Qt version 4.7.0-beta2 and QtMobility version 1.0.2. These need to be preinstalled on your device. You can download the necessary packages here:<br>
<ul><li>Qt 4.7.0-beta2: <a href='http://qt.nokia.com/developer/qt-qtcreator-prerelease#download'>http://qt.nokia.com/developer/qt-qtcreator-prerelease#download</a>
</li><li>QtMobility 1.0.2: <a href='http://qt.nokia.com/products/appdev/add-on-products/catalog/4/new-qt-apis/mobility'>http://qt.nokia.com/products/appdev/add-on-products/catalog/4/new-qt-apis/mobility</a></li></ul>

Questions with respect to installing these packages are best asked on nokia's discussion forums (please search first, before you ask): <a href='http://discussion.forum.nokia.com/forum/forumdisplay.php?196-Qt-for-Symbian'>http://discussion.forum.nokia.com/forum/forumdisplay.php?196-Qt-for-Symbian</a>

<h3>Upgrading</h3>
If you're upgrading from an earlier version of qTracker, then please remove the following files (if they exist):<br>
<ul><li>Default settings file: c:/Data/.config/karpeer.net/qTracker.conf<br>
</li><li>Debug log file: c:/Data/qTracker/debug.txt</li></ul>

This will give you a clean start. If there are still problems, please take a look at <a href='https://code.google.com/p/qtracker/issues/detail?id=3'>Issue 3</a> in the issues list.<br>
<br>
<h3>Installing</h3>
Once you've taken the above steps, you are ready to install the qTracker-x.x.xxx.sis file on your phone. I usually copy it over via bluetooth and install it from the phone file manager, but installing from OVI should work fine as well. My preference is to install it on the E: drive, leaving the precious free Ram space as big as possible.<br>
<br>
Questions? Feel free to contact me at mark.hurenkamp at xs4all.nl