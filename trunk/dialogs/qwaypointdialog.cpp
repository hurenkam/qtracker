#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QStringList>
#include <QLabel>
#include <QLineEdit>
#include <QDoubleValidator>
#include "qwaypointdialog.h"

//QMapSelectionDialog::QMapSelectionDialog(QMapList& maps, QWidget *parent)
QWaypointDialog::QWaypointDialog(QString name, double lat, double lon, QWidget *parent)
    : QDialog(parent)
{
    mainbox =  new QVBoxLayout();
    leftbox =  new QVBoxLayout();
    rightbox = new QVBoxLayout();
    groupbox = new QGroupBox(tr("Add Waypoint:"));
    buttons =  new QHBoxLayout();
    databox = new QHBoxLayout();
    wptname = new QLineEdit(name,this);
    latitude = new QDoubleEdit(lat,this);
    longitude = new QDoubleEdit(lon,this);

    cancel =  new QPushButton(tr("Cancel"));
    confirm = new QPushButton(tr("Confirm"));

    leftbox->addWidget(new QLabel(tr("Name:")));
    leftbox->addWidget(new QLabel(tr("Latitude:")));
    leftbox->addWidget(new QLabel(tr("Longitude:")));
    rightbox->addWidget(wptname);
    rightbox->addWidget(latitude);
    rightbox->addWidget(longitude);
    databox->addLayout(leftbox);
    databox->addLayout(rightbox);
    groupbox->setLayout(databox);

    buttons->addWidget(confirm);
    buttons->addWidget(cancel);
    mainbox->addWidget(groupbox);
    mainbox->addLayout(buttons);
    setLayout(mainbox);

    cancel->show();
    confirm->show();
    setAttribute(Qt::WA_DeleteOnClose);
    //setTitle(tr("Select Point"));

    connect(cancel,SIGNAL(clicked()),this,SLOT(reject()));
    connect(confirm,SIGNAL(clicked()),this,SLOT(accept()));
    //connect(this,SIGNAL(accepted),this,SLOT(emitSelection()));
}

void QWaypointDialog::accept()
{
        QString name = wptname->text();
        double lat = latitude->text().toDouble();
        double lon = longitude->text().toDouble();
        emit confirmed(name,lat,lon);
        QDialog::accept();
        close();
}

QWaypointDialog::~QWaypointDialog()
{
        delete wptname;
        delete latitude;
        delete longitude;
        delete cancel;
        delete confirm;
        delete leftbox;
        delete rightbox;
        delete databox;
        delete buttons;
        delete mainbox;
}
