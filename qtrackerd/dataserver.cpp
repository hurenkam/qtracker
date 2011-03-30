#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlTableModel>
#include <QStringList>
#include "dataserver.h"
#include "track.h"

//#define ENABLE_DEBUG
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

    int platform = -1;
#if   defined(Q_OS_SYMBIAN)
    platform = 0;
#elif defined(Q_WS_MAEMO_5)
    platform = 1;
#elif defined(Q_WS_SIMULATOR)
    platform = 2;
#endif

    db = QSqlDatabase::addDatabase("QSQLITE");
    switch (platform)
    {
    case 0:
        db.setDatabaseName("e:\\data\\qtracker\\database.sqlite");
        break;
    case 2:
        db.setDatabaseName("c:\\data\\qtracker\\database.sqlite");
        break;
    default:
        db.setDatabaseName("database.sqlite");
        break;
    }

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

    if (!db.isOpen()) { EXIT("not open") return 0; }
    Track* result = new Track(db,name,interval);

    EXIT("")
    return result;
}
