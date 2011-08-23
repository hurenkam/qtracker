#ifndef QMLDISTANCEMODEL_H
#define QMLDISTANCEMODEL_H

#include <QStringList>
#include <QUrl>
#include <QObject>

class PrivateDistanceModel;
class qmlDistanceModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int    mask    READ mask    NOTIFY maskChanged)
    Q_PROPERTY(double current READ current NOTIFY currentChanged)
    Q_PROPERTY(double monitor READ monitor NOTIFY monitorChanged)

public:
    int    mask();
    double current();
    double monitor();

    explicit qmlDistanceModel(QObject *parent = 0);
    ~qmlDistanceModel() {}

public slots:
    void reset();

signals:
    void maskChanged();
    void currentChanged();
    void monitorChanged();

private:
    PrivateDistanceModel* p;
    Q_DISABLE_COPY(qmlDistanceModel)
};

#endif // QMLDISTANCEMODEL_H
