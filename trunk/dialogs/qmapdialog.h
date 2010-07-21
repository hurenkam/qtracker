#ifndef QMAPDIALOG_H
#define QMAPDIALOG_H

#include <QListWidget>
#include <QDialog>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QGroupBox>
#include <QLineEdit>
#include "geodata.h"

class QMapTabsDialog : public QDialog
{
    Q_OBJECT

public:
    QMapTabsDialog(WayPoint* p=0, QWidget *parent = 0);

signals:
    void loadmap(QString);
	//void deletemap(QString);
    //void updatemap(QString);

public slots:
    virtual void accept();
};

class QCurrentMapTab: public QWidget
{
    Q_OBJECT

public:
    QCurrentMapTab(QMapTabsDialog *d, QTabWidget* t);
    ~QCurrentMapTab();
    void SetMapName(QString name);

signals:
    void updatemap(QString);

public slots:
    void emitupdatemap()          { emit updatemap(mapname); }
    
protected:
    virtual void resizeEvent(QResizeEvent * event);

private:
	QMapTabsDialog *dialog;
	QTabWidget* tab;
	QString mapname;

	QLineEdit*    name;
	QBoxLayout   *center;
};

class QMapListTab: public QWidget
{
    Q_OBJECT
    
public:
    static const int AllMaps = 0;
    static const int CurrentMaps = 1;
    static const int RecentMaps = 2;

signals:
    void mapselected(QString);

public:
    QMapListTab(QMapTabsDialog *d=0, int tabtype=AllMaps, WayPoint* p=0);
    ~QMapListTab();
    
private slots:
    void confirmselection();
    
private:
    QMapTabsDialog* dialog;
    QListWidget* list;
}; 

#endif // QMAPDIALOG_H
