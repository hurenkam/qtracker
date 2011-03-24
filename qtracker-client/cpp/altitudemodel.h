#ifndef ALTITUDEMODEL_H
#define ALTITUDEMODEL_H

#include <qdeclarative.h>
#include <QStringList>
#include <QUrl>
#include <QObject>
#include <QValueSpaceSubscriber>
#include <QTimer>

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
    void onAltitudeChanged();
    void onReset();

private:
    QValueSpaceSubscriber* _subscriber;
    //QTimer _timer;

    double _hysteresis;
    int    _count;

    void classBegin() {}
    void componentComplete() {}

    Q_DISABLE_COPY(AltitudeModel)
};

QML_DECLARE_TYPE(AltitudeModel)
QT_END_HEADER

#endif // ALTITUDEMODEL_H
