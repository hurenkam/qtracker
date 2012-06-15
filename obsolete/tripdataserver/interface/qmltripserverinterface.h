#ifndef QMLTRIPSERVERINTERFACE_H
#define QMLTRIPSERVERINTERFACE_H

#include <QStringList>
#include <QUrl>
#include <QObject>

class PrivateTripServerInterface;
class qmlTripServerInterface : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int     trip       READ trip       NOTIFY tripChanged)
    Q_PROPERTY(int     track      READ track      NOTIFY trackChanged)
    Q_PROPERTY(QString tripname   READ tripname   NOTIFY tripnameChanged)
    Q_PROPERTY(QString trackname  READ trackname  NOTIFY tracknameChanged)
    Q_PROPERTY(QString trackstate READ trackstate NOTIFY trackstateChanged)

public:
    int     trip();
    int     track();
    QString tripname();
    QString trackname();
    QString trackstate();

    explicit qmlTripServerInterface(QObject *parent = 0);
    ~qmlTripServerInterface() {}

public slots:
    void reset();
    void stop();
    void startTrack(int track, int interval);
    void stopTrack();

signals:
    void tripChanged();
    void trackChanged();
    void tripnameChanged();
    void tracknameChanged();
    void trackstateChanged();

private:
    PrivateTripServerInterface* p;
    Q_DISABLE_COPY(qmlTripServerInterface)
};

#endif // QMLTRIPSERVERINTERFACE_H
