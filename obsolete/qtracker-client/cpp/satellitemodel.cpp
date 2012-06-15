#include <QObject>
#include <QDebug>
#include <QMutableListIterator>
#include "satellitemodel.h"

//#define ENABLE_DEBUG
//#define ENABLE_HEAPINFO
#include "helpers.h"

SatelliteList::SatelliteList(QObject *parent)
: QObject(parent)
{
    ENTER("")

    _satsource = QGeoSatelliteInfoSource::createDefaultSource(this);
    if (_satsource) {
        connect(_satsource, SIGNAL(satellitesInViewUpdated(QList<QGeoSatelliteInfo>)),
                this, SLOT(onInViewChanged(QList<QGeoSatelliteInfo>)));
        connect(_satsource, SIGNAL(satellitesInUseUpdated(QList<QGeoSatelliteInfo>)),
                this, SLOT(onInUseChanged(QList<QGeoSatelliteInfo>)));
        _satsource->startUpdates();
    }

    connect(&_timer, SIGNAL(timeout()), this, SLOT(onTimerExpired()));
    _timer.start(5000);
    EXIT("")
}

QDeclarativeListProperty<SatelliteInfo> SatelliteList::satellites()
{
    ENTER("")
    return QDeclarativeListProperty<SatelliteInfo>(this, _satellites);
}

void SatelliteList::onTimerExpired()
{
    ENTER("")
    // update the satellite list

    // start with removing existing items
    QMutableListIterator<SatelliteInfo*> i(_satellites);
    while (i.hasNext()) {
        SatelliteInfo* s = i.next();
        delete s;
        i.setValue(0);
        i.remove();
    }

    // now fill it with the proper values
    for (int i =0; i<_satsinview.count(); i++)
    {
        double a = _satsinview[i].attribute(QGeoSatelliteInfo::Azimuth);;
        double e = _satsinview[i].attribute(QGeoSatelliteInfo::Elevation);;
        double s = _satsinview[i].signalStrength();

        _satellites.append(new SatelliteInfo(false,s,a,e));
    }

    for (int i =0; i<_satsinuse.count(); i++)
    {
        double a = _satsinuse[i].attribute(QGeoSatelliteInfo::Azimuth);;
        double e = _satsinuse[i].attribute(QGeoSatelliteInfo::Elevation);;
        double s = _satsinuse[i].signalStrength();

        _satellites.append(new SatelliteInfo(true,s,a,e));
    }

    emit satellitesChanged();
    emit countChanged();

    EXIT("")
}

int SatelliteList::satelliteCount() const
{
    ENTER("")
    return _satellites.count();
}

SatelliteInfo *SatelliteList::satellite(int index) const
{
    ENTER("")
    return _satellites.at(index);
}

void SatelliteList::onInUseChanged (const QList<QGeoSatelliteInfo>& inuse)
{
    ENTER("")
    _satsinuse = inuse;
    EXIT("")
}

void SatelliteList::onInViewChanged(const QList<QGeoSatelliteInfo>& inview)
{
    ENTER("")
    _satsinview = inview;
    EXIT("")
}


/*
//============================================================================================


PrivateSatelliteModel::PrivateSatelliteModel(QObject *parent)
    : QObject(parent)
{
    ENTER("")

    satsource = QGeoSatelliteInfoSource::createDefaultSource(this);
    if (satsource) {
        connect(satsource, SIGNAL(satellitesInViewUpdated(QList<QGeoSatelliteInfo>)),
                this, SLOT(onInViewChanged(QList<QGeoSatelliteInfo>)));
        connect(satsource, SIGNAL(satellitesInUseUpdated(QList<QGeoSatelliteInfo>)),
                this, SLOT(onInUseChanged(QList<QGeoSatelliteInfo>)));
        satsource->startUpdates();
    }
    EXIT("")
}

void PrivateSatelliteModel::onInUseChanged (const QList<QGeoSatelliteInfo>& inuse)
{
    ENTER("")
    HEAPINFO(">>")

    emit startUpdate();
    HEAPINFO(">")
    satsinuse = inuse;
    HEAPINFO("<")
    emit endUpdate();

    HEAPINFO("<<")
    EXIT("")
}

void PrivateSatelliteModel::onInViewChanged(const QList<QGeoSatelliteInfo>& inview)
{
    ENTER("")
    HEAPINFO(">>")

    emit startUpdate();
    HEAPINFO(">")
    satsinview = inview;
    HEAPINFO("<")
    emit endUpdate();

    HEAPINFO("<<")
    EXIT("")
}

int PrivateSatelliteModel::inView()
{
    ENTER("")
    RETURN(int, satsinview.count())
}

int PrivateSatelliteModel::inUse()
{
    ENTER("")
    RETURN(int, satsinuse.count())
}

QGeoSatelliteInfo PrivateSatelliteModel::getInView(int i)
{
    ENTER(i)
    RETURN(QGeoSatelliteInfo, satsinview[i])
}

QGeoSatelliteInfo PrivateSatelliteModel::getInUse(int i)
{
    ENTER(i)
    RETURN(QGeoSatelliteInfo, satsinuse[i])
}

//============================================================================================


PrivateSatelliteModel *SatelliteModel::p = 0;

SatelliteModel::SatelliteModel(QObject *parent): QAbstractListModel(parent)
{
    ENTER("")

    QHash<int, QByteArray> roles;
    roles[ElevationRole]      = "elevation";
    roles[AzimuthRole]        = "azimuth";
    roles[SignalStrengthRole] = "strength";
    roles[InUseRole]          = "inuse";
    setRoleNames(roles);

    if (!p) p = new PrivateSatelliteModel();

    connect(p, SIGNAL(startUpdate()), this, SLOT(onUpdateStarted()));
    connect(p, SIGNAL(endUpdate()),  this, SLOT(onUpdateFinished()));

    EXIT("")
}

int SatelliteModel::inView()
{
    ENTER("")
    RETURN(int,p->inView())
}

int SatelliteModel::inUse()
{
    ENTER("")
    RETURN(int,p->inUse())
}

void SatelliteModel::onUpdateStarted()
{
    ENTER("")

    beginResetModel();

    EXIT("")
}

void SatelliteModel::onUpdateFinished()
{
    ENTER("")

    endResetModel();
    //emit countChanged();

    EXIT("")
}

QVariant SatelliteModel::data(const QModelIndex &index, int role) const
{
    QVariant rv;
    ENTER( index.row() << roleNames()[role] )
    HEAPINFO( ">>" << index.row() << roleNames()[role] )
    {
        int i = index.row();
        int v = p->inView();
        int u = p->inUse();
        QGeoSatelliteInfo info;

        if (i < v)
        {
            info = p->getInView(i);
        }
        else if (i-v < u)
        {
            info = p->getInUse(i-v);
        }

        switch (role)
        {
            case SignalStrengthRole:
                rv = info.signalStrength();
                break;
            case AzimuthRole:
                if (info.hasAttribute(QGeoSatelliteInfo::Azimuth))
                    rv = info.attribute(QGeoSatelliteInfo::Azimuth);
                else
                    rv = 0.0;
                break;
            case ElevationRole:
                if (info.hasAttribute(QGeoSatelliteInfo::Elevation))
                    rv = info.attribute(QGeoSatelliteInfo::Elevation);
                else
                    rv = 0.0;
                break;
            case InUseRole:
                rv = (i>=v);
                break;
        }
    }
    HEAPINFO( "<<" << index.row() << roleNames()[role] )
    RETURN(QVariant,rv)
}

int SatelliteModel::rowCount(const QModelIndex &parent) const
{
    ENTER("")

    Q_UNUSED(parent);
    int i = p->inView() + p->inUse();
    return i;

    EXIT("")
}

Q_INVOKABLE bool   SatelliteModel::inuse     (int i)
{
    ENTER("")
    return (i>inView());
}

Q_INVOKABLE double SatelliteModel::azimuth   (int i)
{
    ENTER("")
    QGeoSatelliteInfo info = (i>inView()) ? p->getInUse(i-inView()) : p->getInView(i);
    return info.attribute(QGeoSatelliteInfo::Azimuth);
}

Q_INVOKABLE double SatelliteModel::elevation (int i)
{
    ENTER("")
    QGeoSatelliteInfo info = (i>inView()) ? p->getInUse(i-inView()) : p->getInView(i);
    return info.attribute(QGeoSatelliteInfo::Elevation);
}

Q_INVOKABLE double SatelliteModel::strength  (int i)
{
    ENTER("")
    QGeoSatelliteInfo info = (i>inView()) ? p->getInUse(i-inView()) : p->getInView(i);
    return info.signalStrength();
}
*/
