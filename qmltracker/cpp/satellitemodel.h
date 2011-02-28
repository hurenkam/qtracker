#ifndef SATELLITEMODEL_H
#define SATELLITEMODEL_H

#include <qdeclarative.h>
#include <QStringList>
#include <QUrl>
#include <QObject>
#include <QGeoSatelliteInfo>
#include <QGeoSatelliteInfoSource>
#include <QAbstractListModel>

using namespace QtMobility;

QT_BEGIN_HEADER
QT_MODULE(Declarative)

class QDeclarativeContext;
class QModelIndex;
class PrivateSatelliteModel;

class SatelliteModel : public QAbstractListModel, public QDeclarativeParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QDeclarativeParserStatus)
    Q_PROPERTY(double inview  READ inView  NOTIFY inViewChanged)
    Q_PROPERTY(double inuse   READ inUse   NOTIFY inUseChanged)

signals:
    void inViewChanged();
    void inUseChanged();

public:
    int inView();
    int inUse();

    enum Roles
    {
        SignalStrengthRole = Qt::UserRole+1,
        ElevationRole = Qt::UserRole+2,
        AzimuthRole = Qt::UserRole+3,
        InUseRole = Qt::UserRole+4
    };

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

    explicit SatelliteModel(QObject *parent = 0);
    ~SatelliteModel() {}

private Q_SLOTS:
    //void refresh();
    //void handleDataChanged(const QModelIndex &start, const QModelIndex &end);

    void onInUseChanged()  { beginResetModel(); endResetModel(); emit inUseChanged();  }
    void onInViewChanged() { beginResetModel(); endResetModel(); emit inViewChanged(); }

private:
    static PrivateSatelliteModel *p;

    void classBegin() {}
    void componentComplete() {}

    Q_DISABLE_COPY(SatelliteModel)
};

class PrivateSatelliteModel: public QObject
{
    Q_OBJECT

signals:
    void inViewChanged();
    void inUseChanged();

public:
    explicit PrivateSatelliteModel(QObject *parent=0);

    int inView()                       { return satsinview.count(); }
    int inUse()                        { return satsinuse.count(); }
    QGeoSatelliteInfo getInView(int i) { return satsinview[i]; }
    QGeoSatelliteInfo getInUse(int i)  { return satsinuse[i]; }

private slots:
    void onInUseChanged (const QList<QGeoSatelliteInfo>& inuse)   { satsinuse = inuse;   emit inUseChanged(); }
    void onInViewChanged(const QList<QGeoSatelliteInfo>& inview)  { satsinview = inview; emit inViewChanged(); }

private:
    QGeoSatelliteInfoSource* satsource;
    QList<QGeoSatelliteInfo> satsinuse;
    QList<QGeoSatelliteInfo> satsinview;
};


QML_DECLARE_TYPE(SatelliteModel)
QT_END_HEADER

#endif // SATELLITEMODEL_H
