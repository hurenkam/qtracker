#ifndef QDATUMINTERFACE_H_
#define QDATUMINTERFACE_H_

#include <QStringList>
#include <QWidget>

class QDatumDisplayWidget: public QWidget
{
	Q_OBJECT
public:
	QDatumDisplayWidget(QWidget *parent);
	virtual void setValues(double lat, double lon, int src=0) = 0;
};

class QDatumEditWidget: public QWidget
{
	Q_OBJECT
public:
	QDatumEditWidget(QWidget *parent);
	virtual void setValues(double lat, double lon, int src=0) = 0;
	virtual void values(double& lat, double& lon, int& src) = 0;
};

class QDatumInterface
{
public:
	QDatumInterface();
	virtual ~QDatumInterface();
	
	virtual QStringList DatumList() =0;
	virtual QDatumDisplayWidget *DisplayWidget(QString datum, QWidget *parent=0, int mode=0) = 0;
	virtual QDatumEditWidget *EditWidget(QString datum, QWidget *parent=0, int mode =0) = 0;

public:
    static const int cSourceUnknown = 0;
    static const int cSourceGPS = 1;
    static const int cSourceMap = 2;
};

#endif /* QDATUMINTERFACE_H_ */
