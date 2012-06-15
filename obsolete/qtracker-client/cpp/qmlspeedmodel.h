#ifndef QMLSPEEDMODEL_H
#define QMLSPEEDMODEL_H

#include <QStringList>
#include <QUrl>
#include <QObject>

class PrivateSpeedModel;
class qmlSpeedModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int    mask    READ mask    NOTIFY maskChanged)
    Q_PROPERTY(double current READ current NOTIFY currentChanged)
    Q_PROPERTY(double average READ average NOTIFY averageChanged)
    Q_PROPERTY(double maximum READ maximum NOTIFY maximumChanged)
    Q_PROPERTY(double minimum READ minimum NOTIFY minimumChanged)

    Q_PROPERTY(int    count      READ count      WRITE setCount      NOTIFY countChanged)

public:
    int    mask();
    double current();
    double average();
    double maximum();
    double minimum();

    int    count();

    void setCount(int)         {}

    explicit qmlSpeedModel(QObject *parent = 0);
    ~qmlSpeedModel() {}

public slots:
    void reset();

signals:
    void maskChanged();
    void currentChanged();
    void averageChanged();
    void maximumChanged();
    void minimumChanged();
    void countChanged();

private:
    PrivateSpeedModel* p;
    Q_DISABLE_COPY(qmlSpeedModel)
};

#endif // QMLSPEEDMODEL_H
