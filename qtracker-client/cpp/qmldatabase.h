#ifndef QMLDATABASE_H
#define QMLDATABASE_H

#include <QObject>
#include <QString>
#include <QSqlQuery>
#include <QDateTime>
#include <QDeclarativeListProperty>

class qmlWaypoint: public QObject
{
    Q_OBJECT

public:
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    void     setName(QString value)
    { _name = value; emit nameChanged(); }
    QString  name()
    { return _name; }

    Q_PROPERTY(int wptid READ wptid WRITE setWptid NOTIFY wptidChanged)
    void setWptid(int value)
    { _wptid = value; emit wptidChanged(); }
    int      wptid()
    { return _wptid; }

    Q_PROPERTY(double latitude READ latitude WRITE setLatitude NOTIFY latitudeChanged)
    void setLatitude(double value)
    { _latitude = value; emit latitudeChanged(); }
    double latitude()
    { return _latitude; }

    Q_PROPERTY(double longitude READ longitude WRITE setLongitude NOTIFY longitudeChanged)
    void setLongitude(double value)
    { _longitude = value; emit longitudeChanged(); }
    double longitude()
    { return _longitude; }

    Q_PROPERTY(double altitude READ altitude WRITE setAltitude NOTIFY altitudeChanged)
    void setAltitude(double value)
    { _altitude = value; emit altitudeChanged(); }
    double altitude()
    { return _altitude; }

    qmlWaypoint();
    qmlWaypoint(int id);
    qmlWaypoint(const QSqlQuery& q);

signals:
    void nameChanged();
    void wptidChanged();
    void latitudeChanged();
    void longitudeChanged();
    void altitudeChanged();

private:
    QString    _name;
    int        _wptid;
    double     _latitude;
    double     _longitude;
    double     _altitude;
};

class qmlRoute: public QObject
{
    Q_OBJECT
public:
    Q_PROPERTY(QString name  READ name  WRITE setName  NOTIFY nameChanged)
    void     setName(QString value)
    { _name = value; emit rteidChanged(); }
    QString  name()
    { return _name; }

    Q_PROPERTY(int     rteid READ rteid WRITE setRteid NOTIFY rteidChanged)
    void     setRteid(int value)
    { _rteid = value; }
    int      rteid()
    { return _rteid; }

    Q_PROPERTY(double top READ top WRITE setTop NOTIFY topChanged)
    void setTop(double value)
    { _top = value; emit topChanged(); }
    double top()
    { return _top; }

    Q_PROPERTY(double left READ left WRITE setLeft NOTIFY leftChanged)
    void setLeft(double value)
    { _left = value; emit leftChanged(); }
    double left()
    { return _left; }

    Q_PROPERTY(double bottom READ bottom WRITE setBottom NOTIFY bottomChanged)
    void setBottom(double value)
    { _bottom = value; emit bottomChanged(); }
    double bottom()
    { return _bottom; }

    Q_PROPERTY(double right READ right WRITE setRight NOTIFY rightChanged)
    void setRight(double value)
    { _right = value; emit rightChanged(); }
    double right()
    { return _right; }

    qmlRoute();
    qmlRoute(int id);
    qmlRoute(const QSqlQuery& q);

signals:
    void nameChanged();
    void rteidChanged();
    void topChanged();
    void leftChanged();
    void bottomChanged();
    void rightChanged();

private:
    QString _name;
    int     _rteid;
    double  _top;
    double  _left;
    double  _bottom;
    double  _right;
};

class qmlTrack: public QObject
{
    Q_OBJECT
public:
    Q_PROPERTY(QString name  READ name  WRITE setName  NOTIFY nameChanged)
    void     setName(QString value)
    { _name = value; emit nameChanged(); }
    QString  name()
    { return _name; }

    Q_PROPERTY(int     trkid READ trkid WRITE setTrkid NOTIFY trkidChanged)
    void     setTrkid(int value)
    { _trkid = value; emit trkidChanged(); }
    int      trkid()
    { return _trkid; }

    Q_PROPERTY(double top READ top WRITE setTop NOTIFY topChanged)
    void setTop(double value)
    { _top = value; emit topChanged(); }
    double top()
    { return _top; }

    Q_PROPERTY(double left READ left WRITE setLeft NOTIFY leftChanged)
    void setLeft(double value)
    { _left = value; emit leftChanged(); }
    double left()
    { return _left; }

    Q_PROPERTY(double bottom READ bottom WRITE setBottom NOTIFY bottomChanged)
    void setBottom(double value)
    { _bottom = value; emit bottomChanged(); }
    double bottom()
    { return _bottom; }

    Q_PROPERTY(double right READ right WRITE setRight NOTIFY rightChanged)
    void setRight(double value)
    { _right = value; emit rightChanged(); }
    double right()
    { return _right; }

    qmlTrack();
    qmlTrack(int id);
    qmlTrack(const QSqlQuery& q);

signals:
    void nameChanged();
    void trkidChanged();
    void topChanged();
    void leftChanged();
    void bottomChanged();
    void rightChanged();

private:
    QString _name;
    int     _trkid;
    double  _top;
    double  _left;
    double  _bottom;
    double  _right;
};

class rteProvider: public QObject
{
    Q_OBJECT
public:
    virtual int           rteCount()   =0;
    virtual qmlRoute*     rteAt(int i) =0;
};

class trkProvider: public QObject
{
    Q_OBJECT
public:
    virtual int           trkCount()   =0;
    virtual qmlTrack*     trkAt(int i) =0;
};

class qmlTrip: public QObject
{
    Q_OBJECT

public:
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    void     setName(QString value)
    { _name = value; emit nameChanged(); }
    QString  name()
    { return _name; }

    Q_PROPERTY(int tripid READ tripid WRITE setTripid NOTIFY tripidChanged)
    void     setTripid(int value)
    { _tripid = value; emit tripidChanged(); }
    int      tripid()
    { return _tripid; }

    Q_PROPERTY(int limit READ limit WRITE setLimit NOTIFY limitChanged)
    void     setLimit(int value);
    int      limit()
    { return _limit; }

    Q_PROPERTY(int offset READ offset WRITE setOffset NOTIFY offsetChanged)
    void     setOffset(int value);
    int      offset()
    { return _offset; }

    Q_PROPERTY(QDateTime triptime READ triptime WRITE setTriptime NOTIFY triptimeChanged)
    void setTriptime(QDateTime value)
    { _triptime = value; emit triptimeChanged(); }
    QDateTime triptime()
    { return _triptime; }

    Q_PROPERTY(double tripdist READ tripdist WRITE setTripdist NOTIFY tripdistChanged)
    void setTripdist(double value)
    { _tripdist = value; emit tripdistChanged(); }
    double tripdist()
    { return _tripdist; }

    Q_PROPERTY(double ascent READ ascent WRITE setAscent NOTIFY ascentChanged)
    void setAscent(double value)
    { _ascent = value; emit ascentChanged(); }
    double ascent()
    { return _ascent; }

    Q_PROPERTY(double descent READ descent WRITE setDescent NOTIFY descentChanged)
    void setDescent(double value)
    { _descent = value; emit descentChanged(); }
    double descent()
    { return _descent; }

    Q_PROPERTY(double altmin READ altmin WRITE setAltmin NOTIFY altminChanged)
    void setAltmin(double value)
    { _altmin = value; emit altminChanged(); }
    double altmin()
    { return _altmin; }

    Q_PROPERTY(double altmax READ altmax WRITE setAltmax NOTIFY altmaxChanged)
    void setAltmax(double value)
    { _altmax = value; emit altmaxChanged(); }
    double altmax()
    { return _altmax; }

    Q_PROPERTY(double altavg READ altavg WRITE setAltavg NOTIFY altavgChanged)
    void setAltavg(double value)
    { _altavg = value; emit altavgChanged(); }
    double altavg()
    { return _altavg; }

    Q_PROPERTY(double speedmin READ speedmin WRITE setSpeedmin NOTIFY speedminChanged)
    void setSpeedmin(double value)
    { _speedmin = value; emit speedminChanged(); }
    double speedmin()
    { return _speedmin; }

    Q_PROPERTY(double speedmax READ speedmax WRITE setSpeedmax NOTIFY speedmaxChanged)
    void setSpeedmax(double value)
    { _speedmax = value; emit speedmaxChanged(); }
    double speedmax()
    { return _speedmax; }

    Q_PROPERTY(double speedavg READ speedavg WRITE setSpeedavg NOTIFY speedavgChanged)
    void setSpeedavg(double value)
    { _speedavg = value; emit speedavgChanged(); }
    double speedavg()
    { return _speedavg; }

    Q_PROPERTY(QDeclarativeListProperty<qmlWaypoint> waypoints READ waypoints CONSTANT)
    QDeclarativeListProperty<qmlWaypoint> waypoints();

    Q_PROPERTY(QDeclarativeListProperty<qmlRoute> routes READ routes CONSTANT)
    QDeclarativeListProperty<qmlRoute> routes();

    Q_PROPERTY(QDeclarativeListProperty<qmlTrack> tracks READ tracks CONSTANT)
    QDeclarativeListProperty<qmlTrack> tracks();

    qmlTrip();
    qmlTrip(int id);
    qmlTrip(const QSqlQuery& q);

    Q_INVOKABLE void selectWaypoints(int offset=0, int limit=10);
    Q_INVOKABLE void selectRoutes(int offset=0, int limit=10);
    Q_INVOKABLE void selectTracks(int offset=0, int limit=10);

private:
    void load(const QSqlQuery& q);
    void select();

signals:
    void nameChanged();
    void tripidChanged();
    void limitChanged();
    void offsetChanged();
    void triptimeChanged();
    void tripdistChanged();
    void ascentChanged();
    void descentChanged();
    void altminChanged();
    void altmaxChanged();
    void altavgChanged();
    void speedminChanged();
    void speedmaxChanged();
    void speedavgChanged();

private:
    QString             _name;
    int                 _tripid;
    int                 _limit;
    int                 _offset;
    QDateTime           _triptime;
    double              _tripdist;
    double              _speedavg;
    double              _speedmin;
    double              _speedmax;
    double              _ascent;
    double              _descent;
    double              _altavg;
    double              _altmin;
    double              _altmax;
    QList<qmlWaypoint*> _wpts;
    QList<qmlRoute*>    _rtes;
    QList<qmlTrack*>    _trks;
};

class qmlCategory: public QObject
{
    Q_OBJECT

public:
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    void     setName(QString value)
    { _name = value; emit nameChanged(); }
    QString  name()
    { return _name; }

    Q_PROPERTY(int catid READ catid WRITE setCatid NOTIFY catidChanged)
    void     setCatid(int value)
    { _catid = value; emit catidChanged(); }
    int      catid()
    { return _catid; }

    Q_PROPERTY(int limit READ limit WRITE setLimit NOTIFY limitChanged)
    void     setLimit(int value);
    int      limit()
    { return _limit; }

    Q_PROPERTY(int offset READ offset WRITE setOffset NOTIFY offsetChanged)
    void     setOffset(int value);
    int      offset()
    { return _offset; }

    Q_PROPERTY(QDeclarativeListProperty<qmlWaypoint> waypoints READ waypoints CONSTANT)
    QDeclarativeListProperty<qmlWaypoint> waypoints();

    Q_PROPERTY(QDeclarativeListProperty<qmlRoute> routes READ routes CONSTANT)
    QDeclarativeListProperty<qmlRoute> routes();

    Q_PROPERTY(QDeclarativeListProperty<qmlTrack> tracks READ tracks CONSTANT)
    QDeclarativeListProperty<qmlTrack> tracks();

    qmlCategory();
    qmlCategory(int id);
    qmlCategory(const QSqlQuery& q);

    Q_INVOKABLE void selectWaypoints(int offset=0, int limit=10);
    Q_INVOKABLE void selectRoutes(int offset=0, int limit=10);
    Q_INVOKABLE void selectTracks(int offset=0, int limit=10);

private:
    void load(const QSqlQuery& q);
    void select();

signals:
    void nameChanged();
    void catidChanged();
    void limitChanged();
    void offsetChanged();

private:
    QString             _name;
    int                 _catid;
    int                 _limit;
    int                 _offset;
    QList<qmlWaypoint*> _wpts;
    QList<qmlRoute*>    _rtes;
    QList<qmlTrack*>    _trks;
};

class qmlDatabase: public QObject
{
    Q_OBJECT
public:
    qmlDatabase();

    Q_PROPERTY(QDeclarativeListProperty<qmlCategory> categories READ categories CONSTANT)
    QDeclarativeListProperty<qmlCategory> categories();

    Q_PROPERTY(QDeclarativeListProperty<qmlTrip> trips READ trips CONSTANT)
    QDeclarativeListProperty<qmlTrip> trips();

    Q_PROPERTY(int limit READ limit WRITE setLimit NOTIFY limitChanged)
    void     setLimit(int value);
    int      limit()
    { return _limit; }

    Q_PROPERTY(int offset READ offset WRITE setOffset NOTIFY offsetChanged)
    void     setOffset(int value);
    int      offset()
    { return _offset; }

    Q_INVOKABLE qmlCategory* getCategory(int catid);
    Q_INVOKABLE qmlTrip*     getTrip(int tripid);
    Q_INVOKABLE qmlWaypoint* getWaypoint(int id);
    Q_INVOKABLE qmlRoute*    getRoute(int id);
    Q_INVOKABLE qmlTrack*    getTrack(int id);

signals:
    void limitChanged();
    void offsetChanged();

private:
    void create();
    void select();
    void clear();

private:
    int                 _offset;
    int                 _limit;
    QList<qmlTrip*>     _trips;
    QList<qmlCategory*> _categories;
};

#endif // QMLDATABASE_H
