#ifndef QTRACKDIALOG_H
#define QTRACKDIALOG_H

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
//#include <QDoubleValidator>

class QTrackTabsDialog : public QDialog
{
    Q_OBJECT

public:
    QTrackTabsDialog(Track* track=0, QWidget *parent = 0);
    ~QTrackTabsDialog();

signals:
    void updatetrack(QString, int, int);
    void newtrack(QString, int, int);
    void deletetrack(QString);
    void stoptrack(QString);
    void changeoptions(bool, int, int);

public slots:
    virtual void accept();
    void emitupdatetrack(QString name, int time, int distance);
    void emitnewtrack(QString name, int time, int distance);
    void emitdeletetrack(QString name);
    void emitstoptrack(QString name);
    void emitchangeoptions(bool autostart, int time, int distance);

private:
    QTabWidget       *tabs;
    QDialogButtonBox *buttons;
};

class QNewTrackTab: public QWidget
{
    Q_OBJECT

public:
    QNewTrackTab(QTrackTabsDialog *parent = 0);
    ~QNewTrackTab();

signals:
	void newtrack(QString, int, int);

public slots:
    void emitnewtrack()        { if (trkname) emit newtrack(trkname->text(),time,dist); }
    //void emitupdatetrack()     { if (trkname) emit newtrack(trkname->text(),time,dist); }
    void selectdistance();
    void selecttime();
    void selectall();
    void updatetime(int id);
    void updatedistance(int id);

protected:
    virtual void resizeEvent(QResizeEvent * event);

private:
    int time;
    int dist;
	QBoxLayout *center;
	QGroupBox    *distgroup;
	QButtonGroup *distbuttons;
	QGroupBox    *timegroup;
	QButtonGroup *timebuttons;
	QGroupBox *typegroup;
	QHBoxLayout *namebox;
	QHBoxLayout *buttonbox;
	
	QLineEdit   *trkname;
    QVBoxLayout *main;
};

class QCurrentTrackTab: public QWidget
{
    Q_OBJECT

public:
    QCurrentTrackTab(QString name, QTrackTabsDialog *parent = 0);
    ~QCurrentTrackTab();

signals:
    void updatetrack(QString, int, int);
	void stoptrack(QString);

public slots:
    void emitupdatetrack()        { emit updatetrack(trackname,time,dist); }
    void emitstoptrack()          { emit stoptrack(trackname); }
    void selectdistance();
    void selecttime();
    void selectall();
    void updatetime(int id);
    void updatedistance(int id);

protected:
    virtual void resizeEvent(QResizeEvent * event);

private:
	QString trackname;
    int time;
    int dist;
	QBoxLayout   *center;
	QGroupBox    *distgroup;
	QButtonGroup *distbuttons;
	QGroupBox    *timegroup;
	QButtonGroup *timebuttons;
};

class QTrackListItemWidget: public QListWidgetItem
{
public:
	QTrackListItemWidget(QString name, bool visible=false, QListWidget *parent=0);
	void Show();
	void Hide();
	void Delete();
};

class QTrackListTab: public QWidget
{
    Q_OBJECT

public:
    QTrackListTab(QTrackTabsDialog *parent = 0);
    ~QTrackListTab();

signals:
    void showtrack(Track *);
    void hidetrack(Track *);

public slots:
    void emithidetrack(int id);
    void emitshowtrack(int id);

protected:
    virtual void resizeEvent(QResizeEvent * event);
	QStringList TrackFiles();

private:
	QBoxLayout   *center;
};

#endif // QWAYPOINTDIALOG_H
