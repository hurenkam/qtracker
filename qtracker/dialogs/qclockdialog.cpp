#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QRadioButton>
#include <QGroupBox>
#include <QButtonGroup>
#include <QStringList>
#include <QLabel>
#include <QLineEdit>
#include <QDoubleValidator>
#include <QResizeEvent>
#include <QDateTime>
#include <QToolButton>
#include <QIcon>
#include <QTabWidget>
#include <QFile>
#include <QListWidget>
#include "qclockdialog.h"
#include "qclockwidget.h"
#include "ui.h"
#include "waypointlist.h"
#include "tracklist.h"
#include "routelist.h"
#include "datamonitor.h"

//#include <QDebug>
//#define LOG( a ) qDebug() << a
#define LOG( a ) 

QClockOptions::QClockOptions(QDialog* d)
: QWidget()
, settings("karpeer.net","qTracker",this)
{

    QClockWidget::Type analog = (QClockWidget::Type) settings.value("clock/analog",(int) QClockWidget::CurrentTime).toInt();
    QClockWidget::Type top    = (QClockWidget::Type) settings.value("clock/top",   (int) QClockWidget::TripTime).toInt();
    QClockWidget::Type bottom = (QClockWidget::Type) settings.value("clock/bottom",(int) QClockWidget::ArrivalTime).toInt();

    QGroupBox* analoggroup = new QGroupBox("Analog");
    QGroupBox* topgroup =    new QGroupBox("Top");
    QGroupBox* bottomgroup = new QGroupBox("Bottom");
    analogbuttons = ButtonGroup(analoggroup,analog);
    topbuttons =    ButtonGroup(topgroup,   top);
    bottombuttons = ButtonGroup(bottomgroup,bottom);

    center = new QBoxLayout(QBoxLayout::LeftToRight);
    center->addWidget(analoggroup);
    center->addWidget(topgroup);
    center->addWidget(bottomgroup);

    filler = new QWidget();
    filler->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QPushButton* apply =  new QPushButton(tr("Apply"));
    QPushButton* cancel =  new QPushButton(tr("Cancel"));
    QHBoxLayout* buttonbox =  new QHBoxLayout();
    buttonbox->addWidget(apply);
    buttonbox->addWidget(cancel);

    QVBoxLayout *main = new QVBoxLayout(this);
    main->addLayout(center);
    main->addWidget(filler);
    main->addLayout(buttonbox);
    setLayout(main);

    connect(apply,   SIGNAL(clicked()),this,SLOT(apply()));
    connect(cancel,  SIGNAL(clicked()),d,SLOT(reject()));
}

QButtonGroup* QClockOptions::ButtonGroup(QGroupBox* group, QClockWidget::Type selected)
{
    QButtonGroup* buttons =   new QButtonGroup(group);
    QRadioButton* current =   new QRadioButton("Current");
    QRadioButton* trip =      new QRadioButton("Trip");
    QRadioButton* remaining = new QRadioButton("Remaining");
    QRadioButton* arrival =   new QRadioButton("ETA");
    QVBoxLayout*  box =       new QVBoxLayout();

    buttons->addButton(current,   (int) QClockWidget::CurrentTime);
    buttons->addButton(trip,      (int) QClockWidget::TripTime);
    buttons->addButton(remaining, (int) QClockWidget::RemainingTime);
    buttons->addButton(arrival,   (int) QClockWidget::ArrivalTime);

    box->addWidget(current);
    box->addWidget(trip);
    box->addWidget(remaining);
    box->addWidget(arrival);

    group->setLayout(box);
    group->show();

    QList<QAbstractButton*> list = buttons->buttons();
    for (int i=0; i<list.length(); i++)
        if (buttons->id(list[i]) == (int) selected)
            list[i]->setChecked(true);

    return buttons;
}

QClockOptions::~QClockOptions()
{
}

void QClockOptions::apply()
{
    settings.setValue("clock/analog",analogbuttons->checkedId());
    settings.setValue("clock/top",   topbuttons->checkedId());
    settings.setValue("clock/bottom",bottombuttons->checkedId());

    settings.sync();
    emit changed();
}

void QClockOptions::resizeEvent( QResizeEvent * event )
{
    LOG( "QClockOptions::resizeEvent()\n"; )
    if (!center) return;
    
    if (event->size().width() < event->size().height())
        center->setDirection(QBoxLayout::TopToBottom);
    else
        center->setDirection(QBoxLayout::LeftToRight);

    QWidget::resizeEvent(event);
}

QClockDialog::QClockDialog(QClockWidget *parent)
    //: QDialog(parent)
{
    QFile file(CSSRCDIR "style.css");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    file.close();
    setStyleSheet(styleSheet);

    QTabWidget* tabs = new QTabWidget(this);
    QClockOptions* options = new QClockOptions(this);
    QMonitorOptions* monitor = new QMonitorOptions(this);
    tabs->addTab(options, tr("Clock"));
    tabs->addTab(monitor, tr("Monitor"));
     
    QVBoxLayout *main = new QVBoxLayout();
    main->addWidget(tabs);

    connect(options, SIGNAL(changed()),this,SLOT(accept()));
    connect(monitor, SIGNAL(changed()),this,SLOT(accept()));
    
    setLayout(main);
    move(0,0);
    setModal(true);
    showFullScreen();
    setAttribute(Qt::WA_DeleteOnClose);
}
