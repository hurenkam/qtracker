#ifndef QWAYPOINTDIALOG_H
#define QWAYPOINTDIALOG_H

#include <QListWidget>
#include <QDialog>
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

class QTrackDialog : public QDialog
{
    Q_OBJECT

public:
    QTrackDialog(QString title, QString name, QWidget *parent = 0);
    ~QTrackDialog();

signals:
    void confirmed(QString);

public slots:
    virtual void accept();

private:
    QPushButton *confirm;
    QPushButton *cancel;
    QGroupBox   *groupbox;
    QGridLayout *gridbox;
    QHBoxLayout *buttons;
    QVBoxLayout *mainbox;

    QLineEdit   *trkname;
};

#endif // QWAYPOINTDIALOG_H
