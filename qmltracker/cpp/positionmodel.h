#ifndef POSITIONMODEL_H
#define POSITIONMODEL_H

#include <qdeclarative.h>
#include <QStringList>
#include <QUrl>
#include <QObject>
#include <QGeoPositionInfo>
#include <QGeoPositionInfoSource>

using namespace QtMobility;

QT_BEGIN_HEADER
QT_MODULE(Declarative)

class QDeclarativeContext;
class QModelIndex;
class PrivatePositionModel;

class PositionModel : public QObject, public QDeclarativeParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QDeclarativeParserStatus)
    Q_PROPERTY(double  longitude  READ longitude  NOTIFY longitudeChanged)
    Q_PROPERTY(double  latitude   READ latitude   NOTIFY latitudeChanged)
    Q_PROPERTY(double  altitude   READ altitude   NOTIFY altitudeChanged)
    Q_PROPERTY(double  horizontal READ horizontal NOTIFY horizontalChanged)
    Q_PROPERTY(double  vertical   READ vertical   NOTIFY verticalChanged)
    Q_PROPERTY(QString position   READ position   NOTIFY positionChanged)

public:
    double  latitude();
    double  longitude();
    double  altitude();
    double  vertical();
    double  horizontal();
    QString position();

    explicit PositionModel(QObject *parent = 0);
    ~PositionModel() {}

signals:
    void latitudeChanged();
    void longitudeChanged();
    void altitudeChanged();
    void positionChanged();
    void horizontalChanged();
    void verticalChanged();

private slots:
    void onLatitudeChanged()   { emit longitudeChanged();  }
    void onLongitudeChanged()  { emit latitudeChanged();   }
    void onAltitudeChanged()   { emit altitudeChanged();   }
    void onHorizontalChanged() { emit horizontalChanged(); }
    void onVerticalChanged()   { emit verticalChanged();   }
    void onPositionChanged()   { emit positionChanged();   }

private:
    static PrivatePositionModel *p;

    void classBegin() {}
    void componentComplete() {}

    Q_DISABLE_COPY(PositionModel)
};

class PrivatePositionModel: public QObject
{
    Q_OBJECT

private:
    double _latitude;
    double _longitude;
    double _altitude;
    double _horizontal;
    double _vertical;
    QGeoPositionInfoSource*  possource;
    QGeoPositionInfo         posinfo;

signals:
    void latitudeChanged();
    void longitudeChanged();
    void altitudeChanged();
    void positionChanged();
    void horizontalChanged();
    void verticalChanged();

private:
    void UpdateQuality();

private slots:
    void OnPositionUpdate(const QGeoPositionInfo& info);

public:
    explicit PrivatePositionModel(QObject *parent=0);

    double  latitude()   { return _latitude;   }
    double  longitude()  { return _longitude;  }
    double  altitude()   { return _altitude;   }
    double  horizontal() { return _horizontal; }
    double  vertical()   { return _vertical;   }

    QString position()
    {
        QString s = QString::number(latitude(),'g',8) + "  " + QString::number(longitude(),'g',8);
        return s;
    }
};


QML_DECLARE_TYPE(PositionModel)
QT_END_HEADER

#endif // POSITIONMODEL_H
