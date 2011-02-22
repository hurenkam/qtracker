#ifndef SPEEDMODEL_H
#define SPEEDMODEL_H

#include <qdeclarative.h>
#include <QStringList>
#include <QUrl>
#include <QObject>
#include <QGeoPositionInfo>
#include <QGeoPositionInfoSource>

using namespace QtMobility;

QT_BEGIN_HEADER
QT_MODULE(Declarative)

class QDeclarativeContext;
class QModelIndex;
class PrivateSpeedModel;

class SpeedModel : public QObject, public QDeclarativeParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QDeclarativeParserStatus)
    Q_PROPERTY(double current  READ current  NOTIFY currentChanged)
    Q_PROPERTY(double average  READ average  NOTIFY averageChanged)
    Q_PROPERTY(double max      READ max      NOTIFY maxChanged)
    Q_PROPERTY(double min      READ min      NOTIFY minChanged)
    Q_PROPERTY(double distance READ distance NOTIFY distanceChanged)

    Q_PROPERTY(int    count      READ count      WRITE setCount      NOTIFY countChanged)
    Q_PROPERTY(double hysteresis READ hysteresis WRITE setHysteresis NOTIFY hysteresisChanged)

public:
    double current();
    double average();
    double max();
    double min();
    double distance();
    double hysteresis();
    int    count();

    void setCount(int)         {}
    void setHysteresis(double) {}

    Q_INVOKABLE void reset() { onReset(); }

    explicit SpeedModel(QObject *parent = 0);
    ~SpeedModel() {}

signals:
    void currentChanged();
    void averageChanged();
    void maxChanged();
    void minChanged();
    void distanceChanged();
    void countChanged();
    void hysteresisChanged();

public slots:
    void onReset();

private slots:
    void onCurrentChanged()  { emit currentChanged();  }
    void onAverageChanged()  { emit averageChanged();  }
    void onMaxChanged()      { emit maxChanged();      }
    void onMinChanged()      { emit minChanged();      }
    void onDistanceChanged() { emit distanceChanged(); }

private:
    static PrivateSpeedModel *p;

    void classBegin() {}
    void componentComplete() {}

    Q_DISABLE_COPY(SpeedModel)
};

class PrivateSpeedModel: public QObject
{
    Q_OBJECT

private:
    double _current;
//    double _previous;
    double _hysteresis;
    double _distance;
    double _average;
    double _max;
    double _min;
    int    _count;
    QList<double> _avglist;
    QGeoPositionInfoSource*  possource;
    QGeoPositionInfo         posinfo;
    QGeoPositionInfo         previnfo;
    bool                     prevvalid;

signals:
    void currentChanged();
    void averageChanged();
    void maxChanged();
    void minChanged();
    void distanceChanged();

public slots:
    void onReset();

private slots:
    //void onSpeed();
    void OnPositionUpdate(const QGeoPositionInfo& info);

private:
    void UpdateDistance();
    void UpdateAverage();
    void UpdateMinMax();

public:
    explicit PrivateSpeedModel(QObject *parent=0);

    double getCurrent()    { return _current;    }
    double getHysteresis() { return _hysteresis; }
    double getDistance()   { return _distance;   }
    double getAverage()    { return _average;    }
    double getMax()        { return _max;        }
    double getMin()        { return _min;        }
    int    getCount()      { return _count;      }

    void setHysteresis(double v) { _hysteresis = v; }
    void setCount(int v)         { _count = v;      }
};


QML_DECLARE_TYPE(SpeedModel)
QT_END_HEADER

#endif // SPEEDMODEL_H
