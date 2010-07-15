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
//#include <QDoubleValidator>

class QTrackTabsDialog : public QDialog
{
    Q_OBJECT

public:
    QTrackTabsDialog(QString title, QString name, QWidget *parent = 0);
    ~QTrackTabsDialog();

signals:
    void newtrack(QString, int, int);
    void deletetrack(QString);
    void changeoptions(bool, int, int);

public slots:
    virtual void accept();
    void emitnewtrack(QString name, int time, int distance);
    void emitdeletetrack(QString name);
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
    void emitnewtrack() { if (trkname) emit newtrack(trkname->text(),0,30); }

protected:
    virtual void resizeEvent(QResizeEvent * event);

private:
	QBoxLayout *center;
	QGroupBox *distgroup;
	QGroupBox *typegroup;
	QHBoxLayout *namebox;
	QHBoxLayout *buttonbox;
	
	QLineEdit   *trkname;
    QVBoxLayout *main;
};

#endif // QWAYPOINTDIALOG_H
