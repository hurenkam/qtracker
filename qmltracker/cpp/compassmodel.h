#ifndef COMPASSMODEL_H
#define COMPASSMODEL_H

#include <qdeclarative.h>
#include <QStringList>
#include <QUrl>
#include <QObject>
#include <QCompass>
#include <QCompassReading>

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

private slots:
    void onCurrentChanged() { emit currentChanged(); }
    void onAverageChanged() { emit averageChanged(); }
    void onBearingChanged() { emit bearingChanged(); }
    void onQualityChanged() { emit qualityChanged(); }

private:
    static PrivateCompassModel *p;

    void classBegin() {}
    void componentComplete() {}

    Q_DISABLE_COPY(CompassModel)
};

class PrivateCompassModel: public QObject
{
    Q_OBJECT

private:
    double _current;
    double _bearing;
    double _average;
    double _quality;
    int    _count;
    QList<double> _avglist;
    QCompass*                compass;
    QCompassReading*         reading;

signals:
    void currentChanged();
    void averageChanged();
    void bearingChanged();
    void qualityChanged();

public slots:
    void onReset();

private slots:
    void onReadingChanged();

private:
    void UpdateAverage();
    void UpdateBearing();
    void UpdateQuality();

public:
    explicit PrivateCompassModel(QObject *parent=0);

    double getCurrent()    { return _current;    }
    double getAverage()    { return _average;    }
    double getBearing()    { return _bearing;    }
    double getQuality()    { return _quality;    }
};


QML_DECLARE_TYPE(CompassModel)
QT_END_HEADER
#endif // COMPASSMODEL_H
