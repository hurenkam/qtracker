#include "trackstatus.h"

TrackStatus::TrackStatus(QObject *parent)
    : QObject(parent)
    , id(0)
    , name("trip")
    , state("idle")
{
}

QVariant TrackStatus::toVariant()
{
    QMap<QString, QVariant> r;
    r["id"]    = id;
    r["name"]  = name;
    r["state"] = state;
    return r;
}
