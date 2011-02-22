#ifndef CLOCKMODEL_H
#define CLOCKMODEL_H

#include <qdeclarative.h>
#include <QStringList>
#include <QUrl>
#include <QObject>
#include "datamonitor.h"

QT_BEGIN_HEADER
QT_MODULE(Declarative)

class QDeclarativeContext;
class QModelIndex;
class PrivateClockModel;

class ClockModel : public QObject, public QDeclarativeParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QDeclarativeParserStatus)
    Q_PROPERTY(QDateTime current READ current NOTIFY currentChanged)
    Q_PROPERTY(QDateTime trip    READ trip    NOTIFY tripChanged)

public:
    QDateTime current();
    QDateTime trip();

    Q_INVOKABLE void reset() { onReset(); }

    explicit ClockModel(QObject *parent = 0);
    ~ClockModel() {}

signals:
    void currentChanged();
    void tripChanged();

public slots:
    void onReset();

private slots:
    void onCurrentChanged() { emit currentChanged(); }
    void onTripChanged()    { emit tripChanged();    }

private:
    static PrivateClockModel *p;

    void classBegin() {}
    void componentComplete() {}

    Q_DISABLE_COPY(ClockModel)
};

class PrivateClockModel: public QObject
{
    Q_OBJECT

private:
    QDateTime _current;
    QDateTime _trip;
    QDateTime _start;
    QTimer    timer;

signals:
    void currentChanged();
    void tripChanged();

public slots:
    void onReset();

private slots:
    void onTimerExpired();

private:
    void UpdateTrip();

public:
    explicit PrivateClockModel(QObject *parent=0);

    QDateTime getCurrent()    { return _current;    }
    QDateTime getTrip()       { return _trip;       }
};


QML_DECLARE_TYPE(ClockModel)
QT_END_HEADER

#endif // CLOCKMODEL_H
