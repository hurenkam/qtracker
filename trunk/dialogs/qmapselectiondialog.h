#ifndef QMAPSELECTIONDIALOG_H
#define QMAPSELECTIONDIALOG_H

#include <QListWidget>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

class QMapSelectionDialog : public QDialog
{
    Q_OBJECT

public:
    //QMapSelectionDialog(QMapList& maps, QWidget *parent = 0);
    QMapSelectionDialog(QStringList& files, QWidget *parent = 0);
    ~QMapSelectionDialog();

signals:
        void selectmap(QString);

public slots:
        virtual void accept();

private:
        QPushButton *confirm;
        QPushButton *cancel;
        QHBoxLayout *list;
        QHBoxLayout *buttons;
        QVBoxLayout *main;
        QListWidget *listWidget;
};


#endif // QMAPSELECTIONDIALOG_H
