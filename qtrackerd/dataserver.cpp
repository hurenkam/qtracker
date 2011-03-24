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

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("c:\\data\\qtracker\\database.sqlite");
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
