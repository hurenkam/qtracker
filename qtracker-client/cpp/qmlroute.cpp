#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QStringList>
#include "database.h"
#include "qmldatabase.h"

#define ENABLE_DEBUG
#include "helpers.h"

qmlRoute::qmlRoute()
    : _name("rte")
    , _rteid(0)
{
    ENTER("")
}

qmlRoute::qmlRoute(int id)
{
    ENTER("")
    QSqlDatabase& db = Database::Db();
    QSqlQuery q("SELECT * FROM routes WHERE rte='" + QString::number(id) + "'",db);
    if (q.next())
    {
        _rteid     = getIntField(q,"rte");
        _name      = getStringField(q,"name");
        _top       = getDoubleField(q,"top");
        _left      = getDoubleField(q,"left");
        _bottom    = getDoubleField(q,"bottom");
        _right     = getDoubleField(q,"right");
    }
    EXIT("")
}

qmlRoute::qmlRoute(const QSqlQuery& q)
{
    ENTER("")
    _rteid     = getIntField(q,"rte");
    _name      = getStringField(q,"name");
    _top       = getDoubleField(q,"top");
    _left      = getDoubleField(q,"left");
    _bottom    = getDoubleField(q,"bottom");
    _right     = getDoubleField(q,"right");
    EXIT("")
}
