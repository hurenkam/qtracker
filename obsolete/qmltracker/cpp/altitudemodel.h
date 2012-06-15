#ifndef ALTITUDEMODEL_H
#define ALTITUDEMODEL_H

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
class PrivateAltitudeModel;

class AltitudeModel : public QObject, public QDeclarativeParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QDeclarativeParserStatus)
    Q_PROPERTY(double current READ current NOTIFY currentChanged)
    Q_PROPERTY(double average READ average NOTIFY averageChanged)
    Q_PROPERTY(double ascent  READ ascent  NOTIFY ascentChanged)
    Q_PROPERTY(double descent READ descent NOTIFY descentChanged)
    Q_PROPERTY(double max     READ max     NOTIFY maxChanged)
    Q_PROPERTY(double min     READ min     NOTIFY minChanged)

    Q_PROPERTY(int    count      READ count      WRITE setCount      NOTIFY countChanged)
    Q_PROPERTY(double hysteresis READ hysteresis WRITE setHysteresis NOTIFY hysteresisChanged)

public:
    double current();
    double average();
    double ascent();
    double descent();
    double max();
    double min();
    int    count();
    double hysteresis();
    void setCount(int)         {}
    void setHysteresis(double) {}

    Q_INVOKABLE void reset() { onReset(); }

    explicit AltitudeModel(QObject *parent = 0);
    ~AltitudeModel() {}

signals:
    void currentChanged();
    void averageChanged();
    void ascentChanged();
    void descentChanged();
    void maxChanged();
    void minChanged();
    void countChanged();
    void hysteresisChanged();

public slots:
    void onReset();

private slots:
    void onCurrentChanged() { emit currentChanged(); }
    void onAverageChanged() { emit averageChanged(); }
    void onDescentChanged() { emit descentChanged(); }
    void onAscentChanged()  { emit ascentChanged();  }
    void onMaxChanged()     { emit maxChanged();     }
    void onMinChanged()     { emit minChanged();     }

private:
    static PrivateAltitudeModel *p;

    void classBegin() {}
    void componentComplete() {}

    Q_DISABLE_COPY(AltitudeModel)
};

class PrivateAltitudeModel: public QObject
{
    Q_OBJECT

private:
    double _current;
    double _previous;
    double _hysteresis;
    double _ascent;
    double _descent;
    double _average;
    double _max;
    double _min;
    int    _count;
    QList<double> _avglist;
    QGeoPositionInfoSource*  possource;
    QGeoPositionInfo         posinfo;

signals:
    void currentChanged();
    void averageChanged();
    void ascentChanged();
    void descentChanged();
    void maxChanged();
    void minChanged();

public slots:
    void onReset();

private slots:
    //void onAltitude();
    void OnPositionUpdate(const QGeoPositionInfo& info);

private:
    void UpdateAverage();
    void UpdateAscent();
    void UpdateDescent();
    void UpdateMinMax();

public:
    explicit PrivateAltitudeModel(QObject *parent=0);

    double getCurrent()    { return _current;    }
    double getHysteresis() { return _hysteresis; }
    double getAscent()     { return _ascent;     }
    double getDescent()    { return _descent;    }
    double getAverage()    { return _average;    }
    double getMax()        { return _max;        }
    double getMin()        { return _min;        }
    int    getCount()      { return _count;      }

    void setHysteresis(double v) { _hysteresis = v; }
    void setCount(int v)         { _count = v;      }
};


QML_DECLARE_TYPE(AltitudeModel)
QT_END_HEADER

#endif // ALTITUDEMODEL_H
