#ifndef QMLDATABASE_H
#define QMLDATABASE_H

#include <QObject>
#include <QString>
#include <QSqlQuery>
#include <QDateTime>
#include <QTimer>

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

class qmlDatabasePrivate: public QObject
{
    Q_OBJECT
public:
    qmlDatabasePrivate();
    QSqlDatabase& Db() { return db; }

    void create();
    void select();
    void clear();
    void setLimit(int value);
    void setOffset(int value);

public slots:
    void set(const QString& key, const QVariant& value);
    QVariant get(const QString& key, const QVariant& defaultValue);
    void readSettings();
    void saveSettings();

signals:
    void limitChanged();
    void offsetChanged();
    void settingChanged(QString key, QVariant value);

public:
    QSqlDatabase           db;
    int                    _offset;
    int                    _limit;
    QList<qmlTrip*>        _trips;
    QList<qmlCategory*>    _categories;
    QList<qmlMap*>         _maps;
    QMap<QString,QVariant> _settings;
    QList<QString>         _dirtysettings;
    QTimer                 _timer;
};

class qmlDatabase: public QObject
{
    Q_OBJECT
public:
    qmlDatabase();
    static QSqlDatabase& Db() { if (!p) p = new qmlDatabasePrivate(); return p->Db(); }

    Q_PROPERTY(QDeclarativeListProperty<qmlCategory> categories READ categories CONSTANT)
    QDeclarativeListProperty<qmlCategory> categories();

    Q_PROPERTY(QDeclarativeListProperty<qmlTrip> trips READ trips CONSTANT)
    QDeclarativeListProperty<qmlTrip> trips();

    Q_PROPERTY(QDeclarativeListProperty<qmlMap> maps READ maps CONSTANT)
    QDeclarativeListProperty<qmlMap> maps();

    Q_PROPERTY(int limit READ limit WRITE setLimit NOTIFY limitChanged)
    void     setLimit(int value);
    int      limit();

    Q_PROPERTY(int offset READ offset WRITE setOffset NOTIFY offsetChanged)
    void     setOffset(int value);
    int      offset();

    Q_INVOKABLE qmlCategory* getCategory(int catid);
    Q_INVOKABLE qmlTrip*     getTrip(int tripid);
    Q_INVOKABLE qmlMap*      getMap(int mapid);
    Q_INVOKABLE qmlRefpoint* getRefpoint(int id);
    Q_INVOKABLE qmlWaypoint* getWaypoint(int id);
    Q_INVOKABLE qmlRoute*    getRoute(int id);
    Q_INVOKABLE qmlTrack*    getTrack(int id);

    Q_INVOKABLE void         refresh();

    Q_INVOKABLE void set(const QString& key, const QVariant& value);
    Q_INVOKABLE QVariant get(const QString& key, const QVariant& defaultValue);

signals:
    void limitChanged();
    void offsetChanged();
    void settingChanged(QString key, QVariant value);

private:
    static qmlDatabasePrivate* p;
};

#endif // QMLDATABASE_H
