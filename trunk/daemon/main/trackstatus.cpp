#include "trackstatus.h"

TrackStatus::TrackStatus(QObject *parent)
    : QObject(parent)
    , id(0)
    , name("trip")
    , state("idle")
{
}

void TrackStatus::registerApi(XmlRpcServer* srv)
{
    srv->registerSlot( this,    SLOT( start(const QVariant&, const QVariant&) ), "/RPC2/track/" );
    srv->registerSlot( this,    SLOT( stop() ),                                  "/RPC2/track/" );
    srv->registerSlot( this,    SLOT( data() ),                                  "/RPC2/track/" );
}

QVariant TrackStatus::start(const QVariant& id, const QVariant& name)
{
    return QVariant(0);
}

QVariant TrackStatus::stop()
{
    return QVariant(0);
}

QVariant TrackStatus::data()
{
    QMap<QString, QVariant> r;
    r["id"]    = id;
    r["name"]  = name;
    r["state"] = state;
    return r;
}
