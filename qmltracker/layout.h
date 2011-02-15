#ifndef LAYOUT_H
#define LAYOUT_H

#include <QObject>
#include <QDeclarativeItem>
#include <QDeclarativeListProperty>

class Widget : public QDeclarativeItem
{
    Q_OBJECT
    Q_PROPERTY(int  x READ x WRITE setx)
    Q_PROPERTY(int  y READ y WRITE sety)
    Q_PROPERTY(int  w READ w WRITE setw)
    Q_PROPERTY(int  h READ h WRITE seth)
    Q_PROPERTY(bool v READ v WRITE setv)

public:
    int  x() const    { return _x; }
    int  y() const    { return _y; }
    int  w() const    { return _w; }
    int  h() const    { return _h; }
    void setx(int v)  { _x = v;    }
    void sety(int v)  { _y = v;    }
    void setw(int v)  { _w = v;    }
    void seth(int v)  { _h = v;    }

    bool v() const    { return _v; }
    void setv(bool b) { _v = b;    }

    explicit Widget(QDeclarativeItem *parent=0) :
        QDeclarativeItem(parent)
    {
    }

private:
    int _x,_y,_h,_w;
    bool _v;

signals:

public slots:

};

class Layout : public QDeclarativeItem
{
    Q_OBJECT
    Q_PROPERTY(QDeclarativeListProperty<Widget> landscape_widgets READ landscape_widgets)
    Q_PROPERTY(QDeclarativeListProperty<Widget> portrait_widgets  READ portrait_widgets)

public:
    explicit Layout(QDeclarativeItem *parent = 0);

    QDeclarativeListProperty<Widget> landscape_widgets()
    { return QDeclarativeListProperty<Widget>(this,_landscape); }

    QDeclarativeListProperty<Widget> portrait_widgets()
    { return QDeclarativeListProperty<Widget>(this,_portrait); }

private:
    QList<Widget *> _landscape;
    QList<Widget *> _portrait;

    int landscape_widgetsCount() const
    { return _landscape.count(); }

    int portrait_widgetsCount() const
    { return _portrait.count(); }

    Widget *landscape_widget(int i) const
    { return _landscape.at(i); }

    Widget *portrait_widget(int i) const
    { return _portrait.at(i); }

signals:

public slots:

};

QML_DECLARE_TYPE(Widget)
QML_DECLARE_TYPE(Layout)

#endif // LAYOUT_H
