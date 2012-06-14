#ifndef QCOMPASSDIALOG_H
#define QCOMPASSDIALOG_H

#include <QDialog>
#include <QSettings>

class QGroupBox;
class QButtonGroup;
class QListWidget;
class QComboBox;
class QWidget;
class QRadioButton;
class QHeadingWidget;
class QBoxLayout;
class QCompassDialog : public QDialog
{
    Q_OBJECT
public:
    QCompassDialog(QHeadingWidget *parent = 0);
};

class QCompassOptions: public QWidget
{
	Q_OBJECT

signals:
	void changed();
	
public:
    QCompassOptions(QDialog* d);
    ~QCompassOptions();
    
protected:
    virtual void resizeEvent(QResizeEvent * event);

public slots:
/*
    void noneselected();
    void waypointselected();
    void routeselected();
    void trackselected();
*/    
    void apply();
    
private:
    QSettings settings;
	QBoxLayout* center;
//    QGroupBox* typegroup;
    QButtonGroup* sourcebuttons;
    QButtonGroup* viewbuttons;
//    QButtonGroup* typebuttons;
//    QComboBox* wptcombo;
//    QComboBox* trkcombo;
//    QComboBox* rtecombo;
    QWidget* filler;
//    QStringList list;
//    QString wptname;
//    QString trkname;
//    QString rtename;
//    int montype;
    int source;
    int view;
};


#endif // QCOMPASSDIALOG_H
