#ifndef TRACK_H
#define TRACK_H

#include <QObject>
#include <QSqlTableModel>
#include <QSqlDatabase>
#include "dataserver.h"

class Track : public QObject
{
    Q_OBJECT

friend class DataServer;

signals:
    void areaChanged(double, double, double, double);
    void altitudeChanged(double, double);

public slots:
    void addPoint(double lat, double lon, double alt);

public:
    ~Track();
    int getId()       { return trackid; }
    QString getName() { return trackname; }

private:
    explicit Track(QSqlDatabase& db, QString name, int interval);
    void UpdateMetaData(double lat, double lon, double alt);
    void SetupTracks();
    void SetupTrackPoints();
    void CreateTrack(QString name, int interval);

private:
    QString trackname;
    QSqlDatabase& db;
    QSqlTableModel* tracks;
    QSqlTableModel* trackpoints;
    int trackid;

    bool valid;
    double prevlat;
    double prevlon;
    double top;
    double left;
    double bottom;
    double right;
    double low;
    double high;
};

#endif // TRACK_H
