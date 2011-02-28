#include <QObject>
#include <QDebug>
#include "satellitemodel.h"


//============================================================================================


PrivateSatelliteModel::PrivateSatelliteModel(QObject *parent)
    : QObject(parent)
{
    satsource = QGeoSatelliteInfoSource::createDefaultSource(this);
    if (satsource) {
        connect(satsource, SIGNAL(satellitesInViewUpdated(QList<QGeoSatelliteInfo>)),
                this, SLOT(onInViewChanged(QList<QGeoSatelliteInfo>)));
        connect(satsource, SIGNAL(satellitesInUseUpdated(QList<QGeoSatelliteInfo>)),
                this, SLOT(onInUseChanged(QList<QGeoSatelliteInfo>)));
        satsource->startUpdates();
    }
}

//============================================================================================


PrivateSatelliteModel *SatelliteModel::p = 0;

SatelliteModel::SatelliteModel(QObject *parent): QAbstractListModel(parent)
{
    QHash<int, QByteArray> roles;
    roles[ElevationRole]      = "elevation";
    roles[AzimuthRole]        = "azimuth";
    roles[SignalStrengthRole] = "strength";
    roles[InUseRole]          = "inuse";
    setRoleNames(roles);

    if (!p) p = new PrivateSatelliteModel();

    connect(p, SIGNAL(inViewChanged()), this, SLOT(onInViewChanged()));
    connect(p, SIGNAL(inUseChanged()),  this, SLOT(onInUseChanged()));
}

int SatelliteModel::inView()
{
    return p->inView();
}

int SatelliteModel::inUse()
{
    return p->inUse();
}

QVariant SatelliteModel::data(const QModelIndex &index, int role) const
{
    QVariant rv;/*
    QModelIndex modelIndex = d->model.index(index.row(), 0, d->folderIndex);
    if (modelIndex.isValid()) {
        if (role == FileNameRole)
            rv = d->model.data(modelIndex, QDirModel::FileNameRole).toString();
        else if (role == FilePathRole)
            rv = QUrl::fromLocalFile(d->model.data(modelIndex, QDirModel::FilePathRole).toString());
    }*/
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

    return rv;
}

int SatelliteModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    int i = p->inView() + p->inUse();
    return i;
}
