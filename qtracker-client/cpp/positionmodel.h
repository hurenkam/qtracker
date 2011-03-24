#ifndef POSITIONMODEL_H
#define POSITIONMODEL_H

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
class PrivatePositionModel;

class PositionModel : public QObject, public QDeclarativeParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QDeclarativeParserStatus)
    Q_PROPERTY(double  longitude  READ longitude  NOTIFY longitudeChanged)
    Q_PROPERTY(double  latitude   READ latitude   NOTIFY latitudeChanged)
    Q_PROPERTY(double  altitude   READ altitude   NOTIFY altitudeChanged)
    Q_PROPERTY(double  horizontal READ horizontal NOTIFY horizontalChanged)
    Q_PROPERTY(double  vertical   READ vertical   NOTIFY verticalChanged)
    Q_PROPERTY(QString position   READ position   NOTIFY positionChanged)

public:
    double  latitude();
    double  longitude();
    double  altitude();
    double  vertical();
    double  horizontal();
    QString position();

    explicit PositionModel(QObject *parent = 0);
    ~PositionModel() {}

signals:
    void latitudeChanged();
    void longitudeChanged();
    void altitudeChanged();
    void headingChanged();
    void speedChanged();
    void positionChanged();
    void horizontalChanged();
    void verticalChanged();

private slots:
    void onPositionChanged();

private:
    QValueSpaceSubscriber* _subscriber;
    //QTimer _timer;

    void classBegin() {}
    void componentComplete() {}

    Q_DISABLE_COPY(PositionModel)
};

QML_DECLARE_TYPE(PositionModel)
QT_END_HEADER

#endif // POSITIONMODEL_H
