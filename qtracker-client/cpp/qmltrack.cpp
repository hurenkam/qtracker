#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QStringList>
#include "database.h"
#include "qmldatabase.h"

#define ENABLE_DEBUG
#include "helpers.h"

qmlTrack::qmlTrack()
    : _name("trk")
    , _trkid(0)
{
    ENTER("")
}

qmlTrack::qmlTrack(int id)
{
    ENTER("")
    QSqlDatabase& db = Database::Db();
    QSqlQuery q("SELECT * FROM tracks WHERE trk='" + QString::number(id) + "'",db);
    if (q.next())
    {
        _trkid     = getIntField(q,"trk");
        _name      = getStringField(q,"name");
        _top       = getDoubleField(q,"top");
        _left      = getDoubleField(q,"left");
        _bottom    = getDoubleField(q,"bottom");
        _right     = getDoubleField(q,"right");
    }
    EXIT("")
}

qmlTrack::qmlTrack(const QSqlQuery& q)
{
    ENTER("")
    _trkid     = getIntField(q,"trk");
    _name      = getStringField(q,"name");
    _top       = getDoubleField(q,"top");
    _left      = getDoubleField(q,"left");
    _bottom    = getDoubleField(q,"bottom");
    _right     = getDoubleField(q,"right");
    EXIT("")
}
