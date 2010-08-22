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
#include "qdoubleedit.h"
#include "maplist.h"
#include "geodata.h"

class QMapTabsDialog : public QDialog
{
    Q_OBJECT

public:
    QMapTabsDialog(QWidget *p, MapMetaData* m, RefPoint* r);

signals:
    void loadmap(QString);

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

class QEditRefPointTab: public QWidget
{
    Q_OBJECT

public:
    QEditRefPointTab(QMapTabsDialog *d, QTabWidget* t, MapMetaData* m, RefPoint* r);
    ~QEditRefPointTab();
    
    QString  Name()      { return name->text(); }
    double   Latitude();
    double   Longitude();
    int      X()         { return x->number(); }
    int      Y()         { return y->number(); }
    RefPoint Value()     { return RefPoint(Latitude(),Longitude(),X(),Y()); }

public slots:
    void accept();
    void setvalue(const RefPoint& r);
    
private:
    QSettings			 settings;
	QMapTabsDialog*      dialog;
	QTabWidget*          tab;
	MapMetaData*		 meta;

	QLineEdit*           name;
    QLineEdit*           position;
    QDoubleEdit*		 x;
    QDoubleEdit*		 y;
};

class QDatumTab: public QWidget
{
    Q_OBJECT

public:
    QDatumTab(QMapTabsDialog *d, QTabWidget* t);
    ~QDatumTab();

    geodata::Datum Value() { return datum; }

public slots:
    void accept();
    void setvalue(int v);
    
private:
    QSettings			 settings;
	QMapTabsDialog*      dialog;
	QTabWidget*          tab;
	QButtonGroup*        distbuttons;
	geodata::Datum       datum;
};

#endif // QMAPDIALOG_H
