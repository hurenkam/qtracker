#ifndef QMAPSTATUSBAR_H
#define QMAPSTATUSBAR_H

#include <QToolButton>
#include <QPen>
#include <QString>

class QMapStatusBar : public QToolButton
{
    Q_OBJECT

public:
    QMapStatusBar(QWidget *parent = 0);
    ~QMapStatusBar();
    
public slots:
    void SetPen(const QPen& p)           { pen = p; }
    void SetTopLine(const QString& s)    { top = s; }
    void SetBottomLine(const QString& s) { bottom = s; }

protected:
    virtual void paintEvent(QPaintEvent *event);
    
private:
    QPen pen;    
    QString top;
    QString bottom;
};

#endif // QMAPSTATUSBAR_H
