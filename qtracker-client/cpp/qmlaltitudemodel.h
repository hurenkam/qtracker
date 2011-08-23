#ifndef QMLALTITUDEMODEL_H
#define QMLALTITUDEMODEL_H

#include <QStringList>
#include <QUrl>
#include <QObject>

class PrivateAltitudeModel;
class qmlAltitudeModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int    mask    READ mask    NOTIFY maskChanged)
    Q_PROPERTY(double current READ current NOTIFY currentChanged)
    Q_PROPERTY(double average READ average NOTIFY averageChanged)
    Q_PROPERTY(double maximum READ maximum NOTIFY maximumChanged)
    Q_PROPERTY(double minimum READ minimum NOTIFY minimumChanged)
    Q_PROPERTY(double ascent  READ ascent  NOTIFY ascentChanged)
    Q_PROPERTY(double descent READ descent NOTIFY descentChanged)
    Q_PROPERTY(double monitor READ monitor NOTIFY monitorChanged)

    Q_PROPERTY(int    count      READ count      WRITE setCount      NOTIFY countChanged)
    Q_PROPERTY(double hysteresis READ hysteresis WRITE setHysteresis NOTIFY hysteresisChanged)

public:
    int    mask();
    double current();
    double average();
    double maximum();
    double minimum();
    double ascent();
    double descent();
    double monitor();

    int    count();
    double hysteresis();

    void setCount(int)         {}
    void setHysteresis(double) {}

    explicit qmlAltitudeModel(QObject *parent = 0);
    ~qmlAltitudeModel() {}

public slots:
    void reset();

signals:
    void maskChanged();
    void currentChanged();
    void averageChanged();
    void maximumChanged();
    void minimumChanged();
    void ascentChanged();
    void descentChanged();
    void monitorChanged();
    void countChanged();
    void hysteresisChanged();

private:
    PrivateAltitudeModel* p;
    Q_DISABLE_COPY(qmlAltitudeModel)
};

#endif // QMLALTITUDEMODEL_H
