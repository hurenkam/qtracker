#ifndef QMLDATABASE_H
#define QMLDATABASE_H

#include <QObject>
#include <QString>
#include <QSqlQuery>
#include <QDateTime>
#include <QDeclarativeListProperty>

#include "qmlwaypoint.h"
#include "qmlroute.h"
#include "qmltrack.h"
#include "qmltrip.h"
#include "qmlmap.h"
#include "qmlrefpoint.h"
#include "qmlcategory.h"

extern QString getStringField(const QSqlQuery& q, const QString& fieldName);
extern double getDoubleField(const QSqlQuery& q, const QString& fieldName);
extern QDateTime getDateTimeField(const QSqlQuery& q, const QString& fieldName);
extern int getIntField(const QSqlQuery& q, const QString& fieldName);

class qmlDatabase: public QObject
{
    Q_OBJECT
public:
    qmlDatabase();

    Q_PROPERTY(QDeclarativeListProperty<qmlCategory> categories READ categories CONSTANT)
    QDeclarativeListProperty<qmlCategory> categories();

    Q_PROPERTY(QDeclarativeListProperty<qmlTrip> trips READ trips CONSTANT)
    QDeclarativeListProperty<qmlTrip> trips();

    Q_PROPERTY(QDeclarativeListProperty<qmlMap> maps READ maps CONSTANT)
    QDeclarativeListProperty<qmlMap> maps();

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
    Q_INVOKABLE qmlMap*      getMap(int mapid);
    Q_INVOKABLE qmlRefpoint* getRefpoint(int id);
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
    QList<qmlMap*>      _maps;
};

#endif // QMLDATABASE_H
