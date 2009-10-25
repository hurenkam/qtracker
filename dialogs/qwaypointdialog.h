#ifndef QWAYPOINTDIALOG_H
#define QWAYPOINTDIALOG_H

#include <QListWidget>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QGroupBox>
#include <QLineEdit>
#include <QDoubleValidator>
#include "qmapmetadata.h"

class QDoubleEdit : public QLineEdit
{
public:
    QDoubleEdit::QDoubleEdit(const double input, QWidget *parent) : QLineEdit(QVariant(input).toString(), parent)
    {
        setValidator(new QDoubleValidator(this));
    }
};


class QWaypointDialog : public QDialog
{
    Q_OBJECT

public:
    //QMapSelectionDialog(QMapList& maps, QWidget *parent = 0);
    QWaypointDialog(QString name, double lat, double lon, QWidget *parent = 0);
    ~QWaypointDialog();

signals:
        void confirmed(QString,double,double);

public slots:
        virtual void accept();

private:
        QPushButton *confirm;
        QPushButton *cancel;
        QGroupBox   *groupbox;
        QVBoxLayout *leftbox;
        QVBoxLayout *rightbox;
        QHBoxLayout *buttons;
        QVBoxLayout *mainbox;
        QHBoxLayout *databox;

        QLineEdit   *wptname;
        QDoubleEdit *latitude;
        QDoubleEdit *longitude;
};


#endif // QWAYPOINTDIALOG_H
