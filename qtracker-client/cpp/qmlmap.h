#ifndef QMLMAP_H
#define QMLMAP_H

#include <QObject>
#include <QString>
#include <QSqlQuery>
#include <QDateTime>
#include <QDeclarativeListProperty>

#include "qmlrefpoint.h"

class qmlMap: public QObject
{
    Q_OBJECT
public:
    Q_PROPERTY(QString name  READ name  WRITE setName  NOTIFY nameChanged)
    void     setName(QString value)
    { _name = value; emit nameChanged(); }
    QString  name()
    { return _name; }

    Q_PROPERTY(int mapid READ mapid WRITE setMapid NOTIFY mapidChanged)
    void setMapid(int value)
    { _mapid = value; }
    int mapid()
    { return _mapid; }

    Q_PROPERTY(double top READ top WRITE setTop NOTIFY topChanged)
    void setTop(double value)
    { _top = value; emit topChanged(); }
    double top()
    { return _top; }

    Q_PROPERTY(double left READ left WRITE setLeft NOTIFY leftChanged)
    void setLeft(double value)
    { _left = value; emit leftChanged(); }
    double left()
    { return _left; }

    Q_PROPERTY(double bottom READ bottom WRITE setBottom NOTIFY bottomChanged)
    void setBottom(double value)
    { _bottom = value; emit bottomChanged(); }
    double bottom()
    { return _bottom; }

    Q_PROPERTY(double right READ right WRITE setRight NOTIFY rightChanged)
    void setRight(double value)
    { _right = value; emit rightChanged(); }
    double right()
    { return _right; }

    Q_PROPERTY(QDeclarativeListProperty<qmlRefpoint> refpoints READ refpoints CONSTANT)
    QDeclarativeListProperty<qmlRefpoint> refpoints();

    Q_INVOKABLE void selectRefpoints(int offset, int limit);

    qmlMap();
    qmlMap(int id);
    qmlMap(const QSqlQuery& q);

public slots:
    void save();

private:
    void load(const QSqlQuery& q);

signals:
    void nameChanged();
    void mapidChanged();
    void topChanged();
    void leftChanged();
    void bottomChanged();
    void rightChanged();

private:
    QString _name;
    int     _mapid;
    double  _top;
    double  _left;
    double  _bottom;
    double  _right;
    QList<qmlRefpoint*> _refpts;
};

#endif // QMLMAP_H
