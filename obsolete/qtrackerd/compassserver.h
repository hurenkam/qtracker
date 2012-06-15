#ifndef COMPASSSERVER_H
#define COMPASSSERVER_H

#include <QObject>
#include <QValueSpacePublisher>
#include <QCompass>
#include <QCompassReading>

using namespace QtMobility;

class CompassServer : public QObject
{
    Q_OBJECT
public:
    explicit CompassServer(QObject *parent = 0);
    ~CompassServer();

signals:
    void started();
    void stopped();

public slots:
    void start();
    void stop();
    void reset() {}
    void onReadingChanged();

private:
    QValueSpacePublisher* p;
    QCompass*             c;
    QCompassReading*      r;
};

#endif // COMPASSSERVER_H
