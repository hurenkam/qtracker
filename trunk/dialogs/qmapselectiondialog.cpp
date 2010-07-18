#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QStringList>
#include <QFile>
#include "qmapselectiondialog.h"
#include "ui.h"

//QMapSelectionDialog::QMapSelectionDialog(QMapList& maps, QWidget *parent)
QMapSelectionDialog::QMapSelectionDialog(QStringList& files, QWidget *parent)
    : QDialog(parent)
{
    main = new QVBoxLayout();
    list = new QHBoxLayout();
    buttons = new QHBoxLayout();
    listWidget = new QListWidget();
    cancel = new QPushButton(tr("Cancel"));
    confirm = new QPushButton(tr("Confirm"));

    //QStringList files = maps.keys();
    for (int i = 0; i < files.size(); ++i)
    {
        new QListWidgetItem(files[i], listWidget);
    }

    list->addWidget(listWidget);
    buttons->addWidget(confirm);
    buttons->addWidget(cancel);
    main->addLayout(list);
    main->addLayout(buttons);
    setLayout(main);

    QFile file(CSSRCDIR "style.css");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    setStyleSheet(styleSheet);

    listWidget->show();
    cancel->show();
    confirm->show();
    setAttribute(Qt::WA_DeleteOnClose);
    //setTitle(tr("Select Map"));
    showFullScreen();

    connect(cancel,SIGNAL(clicked()),this,SLOT(reject()));
    connect(confirm,SIGNAL(clicked()),this,SLOT(accept()));
    //connect(this,SIGNAL(accepted),this,SLOT(emitSelection()));
}

void QMapSelectionDialog::accept()
{
    QString filename = listWidget->currentItem()->text();
    emit selectmap(filename);
    QDialog::accept();
    close();
}

QMapSelectionDialog::~QMapSelectionDialog()
{
    delete listWidget;
    delete cancel;
    delete confirm;
    delete list;
    delete buttons;
    delete main;
}
