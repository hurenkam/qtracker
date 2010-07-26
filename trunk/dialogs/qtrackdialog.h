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

class QTrackTabsDialog : public QDialog
{
    Q_OBJECT

public:
    QTrackTabsDialog(Track* track=0, QWidget *parent = 0);

signals:
	void deletetrack(const QString&);
	void showtrack(const QString&);
	void hidetrack(const QString&);
    void updatetrack(QString, int, int);
    void newtrack(QString, int, int);
    void stoptrack(QString);
    void changeoptions(bool, int, int);

public slots:
    virtual void accept();
};

class QCurrentTrackTab: public QWidget
{
    Q_OBJECT

public:
    QCurrentTrackTab(QTrackTabsDialog *d, QTabWidget* t, Track* track=0);
    ~QCurrentTrackTab();
    void SetTrackName(QString name);

signals:
    void newtrack(QString, int, int);
    void updatetrack(QString, int, int);
	void stoptrack(QString);

public slots:
    void emitnewtrack();
    void emitupdatetrack();
    void emitstoptrack();
    void selectdistance();
    void selecttime();
    void selectall();
    void updatetime(int id);
    void updatedistance(int id);
    
protected:
    virtual void resizeEvent(QResizeEvent * event);
    void SwitchToNew();
    void SwitchToCurrent();
    void NewTrackName();

private:
	QTrackTabsDialog *dialog;
	QTabWidget* tab;
	QString trackname;
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

class QTrackListTab: public QWidget
{
    Q_OBJECT

public:
    QTrackListTab(QTrackTabsDialog *parent = 0);
    ~QTrackListTab();
};

class QTrackListWidget: public QWidget
{
	Q_OBJECT

private:
    QSignalMapper* togglemapper;
    QSignalMapper* deletemapper;
    QMap<QString,QHBoxLayout*> items;
    QVBoxLayout* center;

public:
    QTrackListWidget(QTrackListTab* parent=0);

signals:
	void deletetrack(const QString&);
	void showtrack(const QString&);
	void hidetrack(const QString&);

public slots:
    void DeleteTrack(const QString& name);
    void ToggleTrack(const QString& name);
    void TrackVisible(const QString& name);
    void TrackInvisible(const QString& name);
    void TrackDeleted(const QString& name);
    void TrackAdded(const QString& name);
    
protected:
    QStringList TrackFiles();
    QHBoxLayout* NewItem(const QString& name, bool visible);
};

#endif // QWAYPOINTDIALOG_H
