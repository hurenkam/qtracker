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
//#include <QLineEdit>
#include "geodata.h"
#include "qdoubleedit.h"

/*
class QDoubleEdit : public QLineEdit
{
public:
    QDoubleEdit(const double input, QWidget *parent) : QLineEdit(QVariant(input).toString(), parent)
    {
        setValidator(new QDoubleValidator(this));
    }
    double number()                    { return text().toDouble(); }
    void setNumber(const double value) { setText(QVariant(value).toString()); }
};
*/

class QMapTabsDialog : public QDialog
{
    Q_OBJECT

public:
    QMapTabsDialog(QWidget *p, MapMetaData* m, RefPoint* r);

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

class QEditRefPointTab: public QWidget
{
    Q_OBJECT

public:
    QEditRefPointTab(QMapTabsDialog *d, QTabWidget* t, MapMetaData* m, RefPoint* r);
    ~QEditRefPointTab();
    
    QString  Name()      { return name->text(); }
    double   Latitude()  { return latitude->number(); }
    double   Longitude() { return longitude->number(); }
    int      X()         { return x->number(); }
    int      Y()         { return y->number(); }
    RefPoint Value()     { return RefPoint(Latitude(),Longitude(),X(),Y()); }

public slots:
    void accept();
    void setvalue(const RefPoint& r);
    
private:
	QMapTabsDialog*      dialog;
	QTabWidget*          tab;
	MapMetaData*		 meta;

	QLineEdit*           name;
    QDoubleEdit*		 latitude;
    QDoubleEdit*         longitude;
    QDoubleEdit*		 x;
    QDoubleEdit*		 y;
};

#endif // QMAPDIALOG_H
