#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QStringList>
#include "track.h"

//#define ENABLE_DEBUG
#include "helpers.h"

Track::Track(QSqlDatabase& d, QString name, int interval)
    : db(d)
    , valid(false)
{
    ENTER("")
    if (!db.isOpen()) { EXIT("db not open") return; }

    SetupTracks();
    SetupTrackPoints();
    CreateTrack(name,interval);

    EXIT("")
}

Track::~Track()
{
    delete tracks;
    delete trackpoints;
}

void Track::SetupTracks()
{
    ENTER("")

    QSqlQuery query(db);
    if (!db.tables().contains("tracks"))
    {
        QString sql(
                    "CREATE TABLE tracks (trackid INTEGER PRIMARY KEY, name TEXT, interval INTEGER, top REAL, left REAL, bottom REAL, right REAL);"
                    );
        bool result = query.exec (sql);
        if (!result) { EXIT("unable to create tracks table" << db.lastError().text()) return; }
    }
    tracks = new QSqlTableModel(0,db);
    tracks->setTable("tracks");
    tracks->select();

    EXIT("")
}

void Track::SetupTrackPoints()
{
    ENTER("")

    QSqlQuery query(db);
    if (!db.tables().contains("trackpoints"))
    {
        QString sql(
                    "CREATE TABLE trackpoints (pointid INTEGER PRIMARY KEY, trackid INTEGER, latitude REAL, longitude REAL, altitude REAL);"
                    );
        bool result = query.exec (sql);
        if (!result) { EXIT("unable to create trackpoints table" << db.lastError().text()) return; }
    }
    trackpoints = new QSqlTableModel(0,db);
    trackpoints->setTable("trackpoints");
    trackpoints->select();

    EXIT("")
}

void Track::CreateTrack(QString name, int interval)
{
    ENTER("")

    QSqlRecord newtrack = tracks->record();
    newtrack.setValue("name",name);
    newtrack.setValue("interval",interval);
    LOG("newtrack: " << newtrack)
    tracks->insertRecord(-1,newtrack);
    tracks->submit();
    trackid = tracks->query().lastInsertId().toInt();
    QString f = QString("trackid=%1").arg(trackid);
    tracks->setFilter(f);
    tracks->select();
    trackpoints->setFilter(f);
    trackpoints->select();

    EXIT("")
}

void Track::UpdateMetaData(double lat, double lon, double alt)
{
    ENTER("")

    bool areachanged = false;
    bool altchanged = false;

    if (!valid)
    {
        valid = true;
        top = lon;
        bottom = lon;
        left = lat;
        right = lat;
        high = alt;
        low = alt;
        areachanged = true;
        altchanged = true;
    }
    else
    {
        if (lon < top)    { top = lon;     areachanged = true; }
        if (lon > bottom) { bottom = lon;  areachanged = true; }
        if (lat < left)   { left = lat;    areachanged = true; }
        if (lat > right)  { right = lat;   areachanged = true; }
        if (alt > high)   { high = alt;    altchanged = true;  }
        if (alt > low)    { low = alt;     altchanged = true;  }
    }

    if (areachanged)
    {
        QSqlRecord rec = tracks->record(0);
        rec.setValue("top",top);
        rec.setValue("left",left);
        rec.setValue("bottom",bottom);
        rec.setValue("right",right);
        tracks->setRecord(0,rec);
        tracks->submit();
        emit areaChanged(top,left,bottom,right);
    }

    if (altchanged)
    {
        emit altitudeChanged(low,high);
    }

    EXIT("")
}

void Track::addPoint(double lat, double lon, double alt)
{
    ENTER("")

    UpdateMetaData(lat,lon,alt);

    QSqlRecord point = trackpoints->record();
    point.setValue("trackid",trackid);
    point.setValue("latitude",lat);
    point.setValue("longitude",lon);
    point.setValue("altitude",alt);
    trackpoints->insertRecord(-1,point);

    EXIT("")
}
