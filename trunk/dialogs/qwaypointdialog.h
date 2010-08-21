#ifndef QWAYPOINTDIALOG_H
#define QWAYPOINTDIALOG_H

#include <QListWidget>
#include <QDialog>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QGroupBox>
#include <QSettings>
#include "qdoubleedit.h"
#include "waypointlist.h"

class RefPoint;

class QWayPointTabsDialog : public QDialog
{
    Q_OBJECT

public:
    QWayPointTabsDialog(const WayPoint& w, QWidget *p = 0);
    //QWayPointTabsDialog(const RefPoint& r, QWidget *p = 0);

signals:
	void deletewaypoint(const QString&);
	void showwaypoint(const QString&);
	void hidewaypoint(const QString&);
    void updatewaypoint(const QString&);
    void newwaypoint(QString, double, double);
	void editwaypoint(const QString&);

    void newwaypoint(const WayPoint&);
    void newrefpoint(const RefPoint&);

public slots:
    virtual void accept();
};


class QEditWayPointTab: public QWidget
{
    Q_OBJECT

public:
    QEditWayPointTab(QWayPointTabsDialog *d, QTabWidget* t, const WayPoint& w);
    ~QEditWayPointTab();
    
    QString  Name()      { return name->text(); }
    QString  Time()      { return time->text(); }
    double   Latitude()  { return latitude->number(); }
    double   Longitude() { return longitude->number(); }
    double   Elevation() { return elevation->number(); }
    WayPoint Position()  
        { return WayPoint(Latitude(),Longitude(),Elevation(),Time(),Name()); }

public slots:
    void accept();
    void select(const QString& n);
    
private:
    QSettings			 settings;
	QWayPointTabsDialog* dialog;
	QTabWidget*          tab;
	bool				 editmode;
	QString 			 orgname;

	QLineEdit*           name;
	QLineEdit*           time;
    QDoubleEdit*		 latitude;
    QDoubleEdit*         longitude;
    QDoubleEdit*         elevation;
};

class QWayPointListTab: public QWidget
{
    Q_OBJECT

public:
    QWayPointListTab(QWayPointTabsDialog *parent = 0);
    ~QWayPointListTab();
};

class QWayPointListWidget: public QWidget
{
	Q_OBJECT

private:
    QSignalMapper* togglemapper;
    QSignalMapper* deletemapper;
    QSignalMapper* editmapper;
    QMap<QString,QHBoxLayout*> items;
    QVBoxLayout* center;

public:
    QWayPointListWidget(QWayPointListTab* parent=0);

signals:
	void deletewaypoint(const QString&);
	void showwaypoint(const QString&);
	void hidewaypoint(const QString&);
	void editwaypoint(const QString&);

public slots:
    void EditWayPoint(const QString& name);
    void DeleteWayPoint(const QString& name);
    void ToggleWayPoint(const QString& name);
    void WayPointHidden(const QString& name);
    void WayPointVisible(const QString& name);
};

#endif // QWAYPOINTDIALOG_H
