#include "tripstatus.h"

TripStatus::TripStatus(QObject *parent)
  : QObject(parent)
  , id(0)
  , name("trip")
  , state("idle")
{
}

void TripStatus::registerApi(XmlRpcServer* srv)
{
    srv->registerSlot( this,    SLOT( start(const QVariant&, const QVariant&) ), "/RPC2/trip/" );
    srv->registerSlot( this,    SLOT( stop() ),                                  "/RPC2/trip/" );
    srv->registerSlot( this,    SLOT( data() ),                                  "/RPC2/trip/" );
}

QVariant TripStatus::start(const QVariant& id, const QVariant& name)
{
    state = "running";
    this->name = name.toString();
    this->id = id.toInt();
    emit started(this->id,this->name);
    return QVariant(0);
}

QVariant TripStatus::stop()
{
    state = "idle";
    emit stopped();
    return QVariant(0);
}

QVariant TripStatus::data()
{
    QMap<QString, QVariant> r;
    r["id"]    = id;
    r["name"]  = name;
    r["state"] = state;
    return r;
}
