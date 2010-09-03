/*
 * datumlist.cpp
 *
 *  Created on: 31 aug 2010
 *      Author: Mark Hurenkamp
 */

#include "datumlist.h"
#include "projects.h"

Datum* Datum::wgs84;
Datum* Datum::utm;
DatumList* DatumList::instance;
PJ* Datum::ref = pj_init_plus("+proj=latlong +ellps=WGS84 +towgs84=0,0,0 +no_defs");

P4Pos::P4Pos()
: x(0)
, y(0)
, z(0) 
{}

P4Pos::P4Pos(const WayPoint& w)
: x(w.Longitude()*PI/180.0)
, y(w.Latitude()*PI/180.0)
, z(w.Elevation()) 
{}

P4Pos::P4Pos(double lat, double lon, double ele)
: x(lon*PI/180.0)
, y(lat*PI/180.0)
, z(ele) 
{}

double P4Pos::Longitude() 
{ return x*180.0/PI; }

double P4Pos::Latitude()  
{ return y*180.0/PI; }

double P4Pos::Elevation()
{ return z; }

Datum::Datum(QString def, Parser* p): parser(p), self(0)
{
	if (def != "")
	{
		SetProjection(def);
	}
}

Datum::~Datum() 
{
	ClearProjection();
}

void Datum::SetProjection(QString pj)
{
	if (self)
		pj_free(self);
    self = pj_init_plus(pj.toLatin1());
}

void Datum::ClearProjection()
{
	pj_free(self);
	self = 0;
}

bool Datum::HasProjection()
{
    return (self!=0);
}

void Datum::SetAttribute(const QString&, const QString&) {}
QString Datum::GetAttribute(const QString&) { return ""; }
bool Datum::SupportsAttribute(const QString&) { return false; }

QString Datum::Forward(const WayPoint& p) 
{ 
	P4Pos pos(p);
    pj_transform(ref, self, 1,0, &(pos.x),&(pos.y),&(pos.z));
    return parser->fromPos(pos);
}

WayPoint Datum::Inverse(const QString& p)
{
	P4Pos pos = parser->fromString(p);
	pj_transform(self, ref, 1,0, &(pos.x),&(pos.y),&(pos.z));
	return WayPoint(pos.Latitude(), pos.Longitude(), pos.z);
}



UtmDatum::UtmDatum(Parser* p, int z)
: Datum("",p)
, settings("karpeer.net","qTracker",this) 
, zone(z)
{
	GetEllipse();
	GetToWgs();
}

void UtmDatum::SetZone(int z, bool n) 
{ 
	ClearProjection();
	zone=z; 
	north=n; 
}

int UtmDatum::SetZone(const QString& s) 
{
	ClearProjection();
	int p = s.indexOf("N");
	if (p)
	{
		zone = s.left(p).toInt();
		north = true;
	}
	else
	{
		p = s.indexOf("S");
		if (p)
		{
			zone = s.left(p).toInt();
			north = false;
		}
		else
		{
			zone = -1;   
		}
	}
	return p+1;
}

void UtmDatum::SetToWgs(const QString& s)
{
	ClearProjection();
    towgs = s;
	settings.setValue("datum/utm/towgs",s);
	settings.sync();
}

void UtmDatum::SetEllipse(QString s)
{
	ClearProjection();
	ellipse = s;
	settings.setValue("datum/utm/ellps",s);
	settings.sync();
}

QString UtmDatum::GetToWgs()
{
	towgs = settings.value("datum/utm/towgs","0,0,0").toString();
    return towgs;
}

QString UtmDatum::GetEllipse()
{
	ellipse = settings.value("datum/utm/ellps","intl").toString();
	return ellipse;
}

void UtmDatum::SetProjection()
{
	QString pj;
	pj.sprintf("+proj=utm +zone=%i +ellps=",zone);
	pj.append(GetEllipse());
	pj.append(" +towgs=");
	pj.append(GetToWgs());
	pj.append(" +units=m +no_defs");
	Datum::SetProjection(pj);
}

QString UtmDatum::Forward(const WayPoint& p)
{
	if (!HasProjection())
	{
		FindZone(p);
		SetProjection();
	}

	return GetAttribute("zone") % " " % Datum::Forward(p);
}
WayPoint UtmDatum::Inverse(const QString& p)
{
	if (!HasProjection())
		SetProjection();
	
	QString s=p;
	StripZone(s);
	return Datum::Inverse(s);
};

void UtmDatum::SetAttribute(const QString& key, const QString& value) 
{
	if (key == "zone") 		{ SetZone(value); return; }
	if (key == "ellipse")	{ SetEllipse(value); return; }
	if (key == "towgs")     { SetToWgs(value); return; }
	
	Datum::SetAttribute(key,value);
}

QString UtmDatum::GetAttribute(const QString& key) 
{ 
	QString result;
	if (key == "zone")
	{
		if (north)
			return result.sprintf("%iN",zone);
		else
			return result.sprintf("%iS",zone);
	}
	if (key == "ellipse")
	{
		return GetEllipse();
	}
	if (key == "towgs")
		return GetToWgs();
	
	return Datum::GetAttribute(key);
}
	
void UtmDatum::FindZone(const WayPoint& p) 
{
	int ilon = int(floor(p.Longitude()));
	if (ilon >= 180)
		ilon -= 360;
	zone = (ilon + 186)/6;
	north = (p.Latitude()>=0.0);
}
/*	
	  int band = MGRS::LatitudeBand(lat);
	  if (band == 7 && zone == 31 && ilon >= 3)
		zone = 32;
	  else if (band == 9 && ilon >= 0 && ilon < 42)
		zone = 2 * ((ilon + 183)/12) + 1;
*/

void UtmDatum::StripZone(QString& s)
{
	s = s.mid(SetZone(s));
}



DatumList& DatumList::Instance() 
{ 
	if (!instance) 
		new DatumList(); 
	return *instance; 
}

DatumList::DatumList()
: QObject()
, settings("karpeer.net","qTracker",this) 
{ 
	instance = this;
	Datum::wgs84 = new Datum("+proj=latlong +ellps=WGS84 +towgs84=0,0,0 +no_defs",&latlon);
	Datum::utm =   new UtmDatum(&xy);

    AddDatum("UTM",   Datum::utm); 
    AddDatum("Wgs84", Datum::wgs84);
    ReadSettings();
    GetDatum();
}

void DatumList::ReadSettings()
{
	int length = settings.beginReadArray("datum/list");
	for (int i=0; i<length; i++)
	{
		settings.setArrayIndex(i);
		AddDatum(
				settings.value("name").toString(),
				new Datum(
				    settings.value("defenition").toString(),
				    &xy
				    )
		    );
	}
	settings.endArray();
}

void DatumList::SaveSettings()
{
}

DatumList::~DatumList() 
{
	SaveSettings();
}

void DatumList::SetDatum(QString s)
{
	if (!datums.contains(s)) return;
	
	datum = s;
	settings.setValue("datum/selected",s);
	settings.sync();
}

QString DatumList::GetDatum()
{ 
	datum = settings.value("datum/selected","Wgs84").toString();
	if (!datums.contains(datum))
	    datum = "Wgs84";
	
	return datum; 
}

QStringList DatumList::Keys()
{ return datums.keys(); }

QStringList DatumList::Ellipses()
{ 
	QStringList result;
	int i =0;
	while (pj_ellps[i].id != 0)
		result.append(pj_ellps[i++].id);
	return result; 
}

void DatumList::AddDatum(QString name, Datum* d)
{ datums[name] = d; }

QString DatumList::Representation(const WayPoint& w)
{ return datums[datum]->Forward(w); }

WayPoint DatumList::Position(const QString& r)
{ return datums[datum]->Inverse(r); } 
