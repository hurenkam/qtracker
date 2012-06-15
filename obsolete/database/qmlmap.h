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

    Q_PROPERTY(QString filename  READ filename  WRITE setFilename  NOTIFY filenameChanged)
    void     setFilename(QString value)
    { _filename = value; emit filenameChanged(); }
    QString  filename()
    { return _filename; }

    Q_PROPERTY(int mapid READ mapid WRITE setMapid NOTIFY mapidChanged)
    void setMapid(int value);
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

    Q_PROPERTY(bool isCalibrated READ isCalibrated NOTIFY calibrationChanged)
    bool isCalibrated()
    { return _iscalibrated; }

    Q_INVOKABLE bool calibrate();

    Q_PROPERTY(QDeclarativeListProperty<qmlRefpoint> refpoints READ refpoints CONSTANT)
    QDeclarativeListProperty<qmlRefpoint> refpoints();

    Q_INVOKABLE void selectRefpoints(int offset, int limit);

    qmlMap();
    qmlMap(int id);
    qmlMap(const QSqlQuery& q);

public:
    double   mapy2lat(double v)  { return _iscalibrated? _baselat + _dlat/_dy   * (v - _basey) : 0; }
    double   mapx2lon(double v)  { return _iscalibrated? _baselon + _dlon/_dx   * (v - _basex) : 0; }
    double   lat2mapy(double v)  { return _iscalibrated? _basey   + _dy/_dlat * (v - _baselat) : 0; }
    double   lon2mapx(double v)  { return _iscalibrated? _basex   + _dx/_dlon * (v - _baselon) : 0; }

public slots:
    void save();

private:
    void load(const QSqlQuery& q);

signals:
    void nameChanged();
    void filenameChanged();
    void mapidChanged();
    void topChanged();
    void leftChanged();
    void bottomChanged();
    void rightChanged();
    void calibrationChanged();

private:
    QString _name;
    QString _filename;
    int     _mapid;
    double  _top;
    double  _left;
    double  _bottom;
    double  _right;
    double  _baselat;
    double  _baselon;
    double  _basex;
    double  _basey;
    double  _dx;
    double  _dy;
    double  _dlat;
    double  _dlon;
    bool    _iscalibrated;
    QList<qmlRefpoint*> _refpts;
};

#endif // QMLMAP_H
