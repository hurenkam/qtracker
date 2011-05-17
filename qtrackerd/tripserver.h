#ifndef TRIPSERVER_H
#define TRIPSERVER_H

#include <QObject>
#include <QSqlTableModel>
#include <QSqlDatabase>
#include <QValueSpacePublisher>
#include <QTime>
#include <QTimer>
#include "commandhandler.h"

const QString tripInit =
    "CREATE TABLE IF NOT EXISTS trips ("
    "trip INTEGER PRIMARY KEY, name TEXT, "
    "ascent REAL, descent REAL, altmin REAL, altmax REAL, altavg REAL, "
    "speedmin REAL, speedmax REAL, speedavg REAL, triptime TEXT, tripdist REAL );";

using namespace QtMobility;

class TripServer : public CommandHandler
{
    Q_OBJECT

friend class DataServer;

private:
    explicit TripServer(QSqlDatabase& db, QString name="");

public:
    ~TripServer();

    void SetupTrips();
    void CreateTrip(QString name);
    void OpenTrip(int id);

public slots:
    void stop();
    void start();

    void onSpeedMinChanged(double v)     { _dirty = true; speedmin = v; }
    void onSpeedMaxChanged(double v)     { _dirty = true; speedmax = v; }
    void onSpeedAverageChanged(double v) { _dirty = true; speedavg = v; }

    void onAltAscentChanged(double v)    { _dirty = true; altascent = v; }
    void onAltDescentChanged(double v)   { _dirty = true; altdescent = v; }
    void onAltMinChanged(double v)       { _dirty = true; altmin = v; }
    void onAltMaxChanged(double v)       { _dirty = true; altmax = v; }
    void onAltAverageChanged(double v)   { _dirty = true; altavg = v; }

    void onTripTimeChanged(QTime v)      { _dirty = true; triptime = v; }
    void onTripDistanceChanged(double v) { _dirty = true; tripdist = v; }

//private slots:
    void SubmitChangesIfDirty();



private:
    QSqlDatabase&   _db;
    QSqlTableModel* _trips;
    int             _id;
    QString         _name;
    bool            _dirty;
    QTimer          _timer;

    double speedmin;
    double speedmax;
    double speedavg;
    double altascent;
    double altdescent;
    double altmin;
    double altmax;
    double altavg;
    QTime  triptime;
    double tripdist;
};

#endif // TRIPSERVER_H
