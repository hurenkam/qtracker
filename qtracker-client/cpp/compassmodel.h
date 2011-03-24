#ifndef COMPASSMODEL_H
#define COMPASSMODEL_H

#include <qdeclarative.h>
#include <QStringList>
#include <QUrl>
#include <QObject>
#include <QValueSpaceSubscriber>
#include <QValueSpacePublisher>
#include <QTimer>

using namespace QtMobility;

QT_BEGIN_HEADER
QT_MODULE(Declarative)

class QDeclarativeContext;
class QModelIndex;
class PrivateCompassModel;

class CompassModel : public QObject, public QDeclarativeParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QDeclarativeParserStatus)
    Q_PROPERTY(double current READ current NOTIFY currentChanged)
    Q_PROPERTY(double average READ average NOTIFY averageChanged)
    Q_PROPERTY(double bearing READ bearing NOTIFY bearingChanged)
    Q_PROPERTY(double quality READ quality NOTIFY qualityChanged)

public:
    double current();
    double average();
    double bearing();
    double quality();

    Q_INVOKABLE void reset() { onReset(); }

    explicit CompassModel(QObject *parent = 0);
    ~CompassModel() {}

signals:
    void currentChanged();
    void averageChanged();
    void bearingChanged();
    void qualityChanged();

public slots:
    void onReset();

public slots:
    void onAzimuthChanged();
    void onCalibrationChanged();
    void onMonitorChanged();

private:
    QValueSpaceSubscriber* _azimuth;
    QValueSpaceSubscriber* _calibration;
    QValueSpaceSubscriber* _monitor;
    QValueSpacePublisher*  _command;
    //QTimer _timer;

    void classBegin() {}
    void componentComplete() {}

    Q_DISABLE_COPY(CompassModel)
};

QML_DECLARE_TYPE(CompassModel)
QT_END_HEADER
#endif // COMPASSMODEL_H
