/*
 * geodata.h
 *
 *  Created on: 8-jul-2010
 *      Author: Mark Hurenkamp
 */

#ifndef TRACKLIST_H_
#define TRACKLIST_H_

#include <QString>
#include <QList>
#include <QMap>
#include <QGeoPositionInfo>
#include <QGeoPositionInfoSource>
#include <QGeoCoordinate>
#include "ui.h"
#include "geodata.h"

class Track: public QObject
{
    Q_OBJECT
signals:
    void updated(const WayPoint& w);
protected:
	QString name;
	QList<WayPoint*> list;
	//int updatetime;
	//int updatedistance;
public:
	//Track(QString n="", int t=0, int d=0): name(n), updatetime(t), updatedistance(d) {}
	QString Name() const             { return name; }
	QString FileName() const         { return QString(GetDrive() + QString(TRACKDIR) + name + ".gpx"); }
	void SetName(QString n)          { name = n; }
	void AddPoint(WayPoint* w)       { list.append(w); emit updated(*w); }
	WayPoint& GetItem(int i) const   { return *list[i]; }
	int Length() const               { return list.length(); }
};

class TrackList: public QObject
{
    Q_OBJECT
    
signals:
    void added(const QString&);
    void updated(const QString&, const QString&);
    void deleted(const QString&);
    void visible(const QString&);
    void invisible(const QString&);
    
public:
    static TrackList* Instance() { if (!instance) new TrackList(); return instance; }
    
private:
    static TrackList* instance;
    TrackList();
    ~TrackList();
    QStringList FindFiles();
protected:
    QSettings settings;
	QMap<QString, Track*> map;
	QStringList trackfiles;
	QGeoCoordinate prevpos;
	QDateTime prevtime;
	Track* recordtrack;
	bool isrecording;
	int timeinterval;
	int distinterval;
	QGeoPositionInfoSource* possource;

public slots:
	void SaveSettings();
	void Hide(const QString& key);
	void Show(const QString& key);
	void Delete(const QString& key);
	void Start(const QString& key, int d, int t);
	void UpdateInterval(int d, int t);
	void Stop();
	void UpdatePosition(const QGeoPositionInfo& info);
	//void AddTrack(const Track& t)          { AddTrack(new Track(t); }
	//void UpdateTrack(const QString& orgname, const Track& t) {}
	
public:
	// Todo: handle case if name already exists
	void AddTrack(Track* t);
	void AddMetaData(AreaMetaData* m);
	QStringList Keys();
	QStringList VisibleKeys();
	QStringList HiddenKeys();
	QStringList AreaKeys(Bounds a);
	QStringList VisibleAreaKeys(Bounds a);
	bool IsVisible(const QString& k);
	bool IsRecording() { return isrecording; }
	Track& RecordingTrack() { if (!recordtrack) qFatal("recording not available"); return *recordtrack; }
	Track& GetItem(const QString& k) const;
};

#endif /* TRACKLIST_H_ */
