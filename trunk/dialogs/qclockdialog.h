#ifndef QCLOCKDIALOG_H
#define QCLOCKDIALOG_H

#include <QDialog>
#include <QSettings>
#include <QString>
#include "qclockwidget.h"

class QGroupBox;
class QButtonGroup;
class QListWidget;
class QComboBox;
class QWidget;
class QRadioButton;
class QClockWidget;
class QBoxLayout;
class QClockDialog : public QDialog
{
    Q_OBJECT
public:
    QClockDialog(QClockWidget *parent = 0);
};

class QClockOptions: public QWidget
{
	Q_OBJECT

signals:
	void changed();
	
public:
    QClockOptions(QDialog* d);
    ~QClockOptions();
    
protected:
    QButtonGroup* ButtonGroup(QGroupBox* group,QClockWidget::Type selected);
    virtual void resizeEvent(QResizeEvent * event);

public slots:
    void apply();
    
private:
    QSettings settings;
	QBoxLayout* center;
    QButtonGroup* analogbuttons;
    QButtonGroup* topbuttons;
    QButtonGroup* bottombuttons;
    QWidget* filler;
    QClockWidget::Type analog;
    QClockWidget::Type top;
    QClockWidget::Type bottom;
};


#endif // QCLOCKDIALOG_H
