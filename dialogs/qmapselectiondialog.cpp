#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushBUtton>
#include <QStringList>
#include "qmapselectiondialog.h"

QMapSelectionDialog::QMapSelectionDialog(QMapList& maps, QWidget *parent)
    : QDialog(parent)
{
    QVBoxLayout *main = new QVBoxLayout();
    QHBoxLayout *list = new QHBoxLayout();
    QHBoxLayout *buttons = new QHBoxLayout();
    listWidget = new QListWidget();
    QPushButton *cancel = new QPushButton(tr("Cancel"));
    QPushButton *confirm = new QPushButton(tr("Confirm"));

    QStringList files = maps.keys();
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

    listWidget->show();
    cancel->show();
    confirm->show();
    setAttribute(Qt::WA_DeleteOnClose);

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
}
