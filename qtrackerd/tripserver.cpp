#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>
#include <QStringList>
#include "tripserver.h"

#define ENABLE_DEBUG
#include "helpers.h"

TripServer::TripServer(QSqlDatabase& db, QString)
    : _db(db)
    , _dirty(false)
    , _trips(0)
    , _id(0)
{
    ENTER("")
    if (!_db.isOpen()) { EXIT("db not open") return; }

    SetupTrips();
    start();

    EXIT("")
}

TripServer::~TripServer()
{
    SubmitChangesIfDirty();
    delete _trips;
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
    _timer.stop();
    _dirty = false;
}

void TripServer::start()
{
    stop();
    QString name = "trip-" + QDateTime().currentDateTime().toString("yyyyMMdd-hhmmss");
    CreateTrip(name);
    _timer.start(60000);
    connect(&_timer,SIGNAL(timeout()),this,SLOT(SubmitChangesIfDirty()));
}

void TripServer::CreateTrip(QString name)
{
    ENTER("")

    QSqlRecord newtrip = _trips->record();
    newtrip.setValue("name",name);
    LOG("newtrip: " << newtrip)
    _trips->insertRecord(-1,newtrip);
    _trips->submit();
    int id = _trips->query().lastInsertId().toInt();
    OpenTrip(id);

    EXIT("")
}

void TripServer::OpenTrip(int id)
{
    ENTER("")

    _id = id;
    QString f = QString("trip=%1").arg(_id);
    _trips->setFilter(f);
    _trips->select();

    EXIT("")
}
