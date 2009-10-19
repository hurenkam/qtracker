#ifndef QMAPSELECTIONDIALOG_H
#define QMAPSELECTIONDIALOG_H

#include <QListWidget>
#include <QDialog>
#include "qmapmetadata.h"

class QMapSelectionDialog : public QDialog
{
    Q_OBJECT

public:
    QMapSelectionDialog(QMapList& maps, QWidget *parent = 0);
    ~QMapSelectionDialog();

signals:
        void selectmap(QString);

public slots:
        virtual void accept();

private:
        QListWidget *listWidget;
};


#endif // QMAPSELECTIONDIALOG_H
