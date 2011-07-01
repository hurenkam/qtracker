#ifndef QMLREFPOINT_H
#define QMLREFPOINT_H

#include <QObject>
#include <QString>
#include <QSqlQuery>
#include <QDateTime>
#include <QDeclarativeListProperty>

class qmlRefpoint: public QObject
{
    Q_OBJECT

public:
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    void     setName(QString value)
    { _name = value; emit nameChanged(); }
    QString  name()
    { return _name; }

    Q_PROPERTY(int refid READ refid WRITE setRefid NOTIFY refidChanged)
    void setRefid(int value)
    { _refid = value; emit refidChanged(); }
    int      refid()
    { return _refid; }

    Q_PROPERTY(double latitude READ latitude WRITE setLatitude NOTIFY latitudeChanged)
    void setLatitude(double value)
    { _latitude = value; emit latitudeChanged(); }
    double latitude()
    { return _latitude; }

    Q_PROPERTY(double longitude READ longitude WRITE setLongitude NOTIFY longitudeChanged)
    void setLongitude(double value)
    { _longitude = value; emit longitudeChanged(); }
    double longitude()
    { return _longitude; }

    Q_PROPERTY(int x READ x WRITE setX NOTIFY xChanged)
    void setX(int value)
    { _x = value; emit xChanged(); }
    int x()
    { return _x; }

    Q_PROPERTY(int y READ y WRITE setY NOTIFY yChanged)
    void setY(int value)
    { _y = value; emit yChanged(); }
    int y()
    { return _y; }

    qmlRefpoint();
    qmlRefpoint(int id);
    qmlRefpoint(const QSqlQuery& q);

signals:
    void nameChanged();
    void refidChanged();
    void latitudeChanged();
    void longitudeChanged();
    void xChanged();
    void yChanged();

private:
    QString    _name;
    int        _refid;
    double     _latitude;
    double     _longitude;
    int        _x;
    int        _y;
};

#endif // QMLREFPOINT_H
