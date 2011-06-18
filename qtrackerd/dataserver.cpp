#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlTableModel>
#include <QStringList>
#include <QDir>
#include "dataserver.h"
#include "track.h"
#include "tripserver.h"

#define ENABLE_DEBUG
#include "helpers.h"

DataServer::DataServer(QObject *parent)
    : QObject(parent)
{
    ENTER("")
    EXIT("")
}

DataServer::~DataServer()
{
    ENTER("")
    EXIT("")
}

void DataServer::start()
{
    ENTER("")

    db = QSqlDatabase::addDatabase("QSQLITE");

#if   defined(Q_OS_SYMBIAN)
    if (! QDir("e:/data/qtracker/").exists() )
        QDir().mkdir("e:/data/qtracker/");
    db.setDatabaseName("e:\\data\\qtracker\\database.sqlite");
#elif defined(Q_WS_MAEMO_5)
    db.setDatabaseName("database.sqlite");
#elif defined(Q_WS_SIMULATOR)
    if (! QDir("c:/data/").exists() )
        QDir().mkdir("c:/data/");
    if (! QDir("c:/data/qtracker/").exists() )
        QDir().mkdir("c:/data/qtracker/");
    db.setDatabaseName("c:\\data\\qtracker\\database.sqlite");
#else
    db.setDatabaseName("database.sqlite");
#endif

    bool result = db.open();
    if (!result) { EXIT("unable to open db" << error().text()) return; }

    EXIT("")
}

void DataServer::stop()
{
    ENTER("")
    db.close();
    EXIT("")
}

QSqlError DataServer::error()
{
    ENTER("")
    return db.lastError();
}

Track* DataServer::createTrack(QString name, int interval)
{
    ENTER("")

    if (!db.isOpen()) { EXIT("db not open") return 0; }
    Track* result = new Track(db,name,interval);

    EXIT("")
    return result;
}

Track* DataServer::openTrack(int id)
{
    ENTER("")

    if (!db.isOpen()) { EXIT("db not open") return 0; }
    Track* result = new Track(db,id);

    EXIT("")
    return result;
}

TripServer* DataServer::getTripServer(QString name)
{
    ENTER("")

    if (!db.isOpen()) { EXIT("db not open") return 0; }
    TripServer* result = new TripServer(db,name);

    EXIT("")
    return result;
}
