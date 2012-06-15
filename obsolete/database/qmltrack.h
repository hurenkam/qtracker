#ifndef QMLTRACK_H
#define QMLTRACK_H

#include <QObject>
#include <QString>
#include <QSqlQuery>
#include <QDateTime>
//#include <QDeclarativeListProperty>

class qmlTrack: public QObject
{
    Q_OBJECT
public:
    Q_PROPERTY(QString name  READ name  WRITE setName  NOTIFY nameChanged)
    void     setName(QString value)
    { _name = value; emit nameChanged(); }
    QString  name()
    { return _name; }

    Q_PROPERTY(int     trkid READ trkid WRITE setTrkid NOTIFY trkidChanged)
    void     setTrkid(int value)
    { _trkid = value; emit trkidChanged(); }
    int      trkid()
    { return _trkid; }

    Q_PROPERTY(int interval READ interval WRITE setInterval NOTIFY intervalChanged)
    void     setInterval(int value)
    { _interval = value; emit intervalChanged(); }
    int      interval()
    { return _interval; }

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

    qmlTrack();
    qmlTrack(int id);
    qmlTrack(const QSqlQuery& q);

public slots:
    void addTrackPoint(double lat, double lon, double alt);
    void save();

private:
    void load(const QSqlQuery& q);

signals:
    void nameChanged();
    void trkidChanged();
    void topChanged();
    void leftChanged();
    void bottomChanged();
    void rightChanged();
    void intervalChanged();

private:
    QString _name;
    int     _trkid;
    double  _top;
    double  _left;
    double  _bottom;
    double  _right;
    int     _interval;
};

#endif // QMLTRACK_H
