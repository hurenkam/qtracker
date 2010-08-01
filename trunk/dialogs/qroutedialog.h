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

public slots:
    virtual void accept();
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
