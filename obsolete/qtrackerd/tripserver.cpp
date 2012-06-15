#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>
#include <QStringList>
#include "tripserver.h"

#define ENABLE_DEBUG
#include "helpers.h"

TripServer::TripServer(QSqlDatabase& db, QString)
    : _p(0)
    , _db(db)
    , _dirty(false)
    , _trips(0)
    , _id(0)
    , altascent(0.0)
    , altdescent(0.0)
    , altmin(0.0)
    , altmax(0.0)
    , altavg(0.0)
    , speedmin(0.0)
    , speedmax(0.0)
    , speedavg(0.0)
    , triptime(0,0,0)
    , tripdist(0.0)
{
    ENTER("")
    if (!_db.isOpen()) { EXIT("db not open") return; }

    methods << "stop" << "start";
    SetupTrips();

    EXIT("")
}

TripServer::~TripServer()
{
    SubmitChangesIfDirty();
    delete _trips;
}

void TripServer::onCommand(int method, QVariantList args)
{
    ENTER("")

    switch (method)
    {
        case 0:
            stop();
            break;
        case 1:
            start();
            break;
    }

    EXIT("")
}

void TripServer::publishTripTime()
{
    if (!_p) return;

    QVariantMap vt;
    vt["hour"]   = triptime.hour();
    vt["minute"] = triptime.minute();
    vt["second"] = triptime.second();

    _p->setValue("time",vt);
}

void TripServer::publishDistance()
{
    if (!_p) return;
    _p->setValue("distance",tripdist);
}

void TripServer::SubmitChangesIfDirty()
{
    if (!_dirty) return;
    ENTER("")

    QSqlRecord rec = _trips->record(0);
    rec.setValue( "ascent",   altascent  );
    rec.setValue( "descent",  altdescent );
    rec.setValue( "altmin",   altmin     );
    rec.setValue( "altmax",   altmax     );
    rec.setValue( "altavg",   altavg     );
    rec.setValue( "speedmin", speedmin   );
    rec.setValue( "speedmax", speedmax   );
    rec.setValue( "speedavg", speedavg   );
    rec.setValue( "triptime", triptime.toString("hh:mm:ss") );
    rec.setValue( "tripdist", tripdist   );
    _trips->setRecord(0,rec);
    _trips->submit();

    EXIT("")
}

void TripServer::SetupTrips()
{
    ENTER("")

    QSqlQuery query(_db);
    if (!_db.tables().contains("trips"))
    {
        QString sql(tripInit);
        bool result = query.exec (sql);
        if (!result) { EXIT("unable to create trips table" << _db.lastError().text()) return; }
    }
    _trips = new QSqlTableModel(0,_db);
    _trips->setTable("trips");
    _trips->select();

    EXIT("")
}

void TripServer::stop()
{
    if (_p) _p->setValue("status","stopped");

    _timer.stop();
    _dirty = false;

    emit stopped();
}

void TripServer::start()
{
    if (!_p) _p = new QValueSpacePublisher("/server/trip");

    stop();
    QString name = "trip-" + QDateTime().currentDateTime().toString("yyyyMMdd-hhmmss");
    CreateTrip(name);
    _timer.start(60000);
    _starttime = QDateTime::currentDateTime().time();

    connect(&_timer,SIGNAL(timeout()),this,SLOT(SubmitChangesIfDirty()));
    _p->setValue("status",QString("started"));
    publishTripTime();
    publishDistance();

    emit started();
}

void TripServer::CreateTrip(QString name)
{
    ENTER("")
    if (_p) _p->setValue("name",name);

    QSqlRecord newtrip = _trips->record();
    newtrip.setValue("name",name);
    LOG("newtrip: " << newtrip)
    _trips->insertRecord(-1,newtrip);
    _trips->submit();
    _name = name;

    int id = _trips->query().lastInsertId().toInt();
    OpenTrip(id);

    EXIT("")
}

void TripServer::OpenTrip(int id)
{
    ENTER("")
    if (_p) _p->setValue("id",id);

    _id = id;
    QString f = QString("trip=%1").arg(_id);
    _trips->setFilter(f);
    _trips->select();

    EXIT("")
}
