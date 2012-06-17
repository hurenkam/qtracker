#include "tripstatus.h"

TripStatus::TripStatus(QObject *parent)
  : QObject(parent)
  , id(0)
  , name("trip")
  , state("idle")
{
}

QVariant TripStatus::start(QVariant id, QVariant name)
{
    return QVariant(0);
}

QVariant TripStatus::stop();
{
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
