#ifndef QMLROUTE_H
#define QMLROUTE_H

#include <QObject>
#include <QString>
#include <QSqlQuery>
#include <QDateTime>
#include <QDeclarativeListProperty>

class qmlRoute: public QObject
{
    Q_OBJECT
public:
    Q_PROPERTY(QString name  READ name  WRITE setName  NOTIFY nameChanged)
    void     setName(QString value)
    { _name = value; emit nameChanged(); }
    QString  name()
    { return _name; }

    Q_PROPERTY(int     rteid READ rteid WRITE setRteid NOTIFY rteidChanged)
    void     setRteid(int value)
    { _rteid = value; emit rteidChanged(); }
    int      rteid()
    { return _rteid; }

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

    qmlRoute();
    qmlRoute(int id);
    qmlRoute(const QSqlQuery& q);

signals:
    void nameChanged();
    void rteidChanged();
    void topChanged();
    void leftChanged();
    void bottomChanged();
    void rightChanged();

private:
    QString _name;
    int     _rteid;
    double  _top;
    double  _left;
    double  _bottom;
    double  _right;
};

#endif // QMLROUTE_H
