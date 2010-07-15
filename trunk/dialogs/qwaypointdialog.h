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
#include <QLineEdit>
#include <QDoubleValidator>

class QDoubleEdit : public QLineEdit
{
public:
    QDoubleEdit(const double input, QWidget *parent) : QLineEdit(QVariant(input).toString(), parent)
    {
        setValidator(new QDoubleValidator(this));
    }
};


class QWaypointDialog : public QDialog
{
    Q_OBJECT

public:
    QWaypointDialog(QString title, QString name, double lat, double lon, QWidget *parent = 0);
    ~QWaypointDialog();

signals:
    void confirmed(QString,double,double);

public slots:
    virtual void accept();

private:
    QPushButton *confirm;
    QPushButton *cancel;
    QGroupBox   *groupbox;
    QGridLayout *gridbox;
    QHBoxLayout *buttons;
    QVBoxLayout *mainbox;

    QLineEdit   *wptname;
    QDoubleEdit *latitude;
    QDoubleEdit *longitude;
};

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
