#ifndef QROUTEDIALOG_H
#define QROUTEDIALOG_H

#include <QListWidget>
#include <QDialog>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QGroupBox>
#include <QLineEdit>
#include "routelist.h"

class QRouteTabsDialog : public QDialog
{
    Q_OBJECT

public:
    QRouteTabsDialog(Route* route=0, QWidget *parent = 0);

signals:
	void deleteroute(const QString&);
	void showroute(const QString&);
	void hideroute(const QString&);
    void updateroute(QString, int, int);
    void newroute(QString, int, int);
    void stoproute(QString);
    void changeoptions(bool, int, int);

public slots:
    virtual void accept();
};

class QCurrentRouteTab: public QWidget
{
    Q_OBJECT

public:
    QCurrentRouteTab(QRouteTabsDialog *d, QTabWidget* t, Route* route=0);
    ~QCurrentRouteTab();
    void SetRouteName(QString name);

signals:
    void newroute(QString, int, int);
    void updateroute(QString, int, int);
	void stoproute(QString);

public slots:
    void emitnewroute();
    void emitupdateroute();
    void emitstoproute();
    void selectdistance();
    void selecttime();
    void selectall();
    void updatetime(int id);
    void updatedistance(int id);
    
protected:
    virtual void resizeEvent(QResizeEvent * event);
    void SwitchToNew();
    void SwitchToCurrent();
    void NewRouteName();

private:
	QRouteTabsDialog *dialog;
	QTabWidget* tab;
	QString routename;
    int time;
    int dist;
	QLineEdit*    trkname;
	QPushButton*  stop;
	QPushButton*  upd;
	QPushButton*  start;
	QBoxLayout   *center;
	QGroupBox    *distgroup;
	QButtonGroup *distbuttons;
	QGroupBox    *timegroup;
	QButtonGroup *timebuttons;
};

class QRouteListTab: public QWidget
{
    Q_OBJECT

public:
    QRouteListTab(QRouteTabsDialog *parent = 0);
    ~QRouteListTab();
};

class QRouteListWidget: public QWidget
{
	Q_OBJECT

private:
    QSignalMapper* togglemapper;
    QSignalMapper* deletemapper;
    QMap<QString,QHBoxLayout*> items;
    QVBoxLayout* center;

public:
    QRouteListWidget(QRouteListTab* parent=0);

signals:
	void deleteroute(const QString&);
	void showroute(const QString&);
	void hideroute(const QString&);

public slots:
    void DeleteRoute(const QString& name);
    void ToggleRoute(const QString& name);
    void RouteVisible(const QString& name);
    void RouteInvisible(const QString& name);
    void RouteDeleted(const QString& name);
    void RouteAdded(const QString& name);
    
protected:
    QStringList RouteFiles();
    QHBoxLayout* NewItem(const QString& name, bool visible);
};

#endif // QROUTEDIALOG_H
