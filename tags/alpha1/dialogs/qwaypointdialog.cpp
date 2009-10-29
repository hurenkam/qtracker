#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QStringList>
#include <QLabel>
#include <QLineEdit>
#include <QDoubleValidator>
#include "qwaypointdialog.h"

QWaypointDialog::QWaypointDialog(QString title, QString name, double lat, double lon, QWidget *parent)
    : QDialog(parent)
{
    mainbox =  new QVBoxLayout();
    gridbox = new QGridLayout();
    groupbox = new QGroupBox(title);
    groupbox->setLayout(gridbox);
    buttons =  new QHBoxLayout();
    wptname = new QLineEdit(name,this);
    latitude = new QDoubleEdit(lat,this);
    longitude = new QDoubleEdit(lon,this);

    cancel =  new QPushButton(tr("Cancel"));
    confirm = new QPushButton(tr("Confirm"));

    gridbox->addWidget(new QLabel(tr("Name:")),0,0);
    gridbox->addWidget(new QLabel(tr("Latitude:")),1,0);
    gridbox->addWidget(new QLabel(tr("Longitude:")),2,0);
    gridbox->addWidget(wptname,0,1);
    gridbox->addWidget(latitude,1,1);
    gridbox->addWidget(longitude,2,1);

    buttons->addWidget(confirm);
    buttons->addWidget(cancel);
    mainbox->addWidget(groupbox);
    mainbox->addLayout(buttons);
    setLayout(mainbox);

    cancel->show();
    confirm->show();
    setAttribute(Qt::WA_DeleteOnClose);

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
    delete buttons;
    delete gridbox;
    delete mainbox;
}
