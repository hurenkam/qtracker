#ifndef DATASERVER_H
#define DATASERVER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlError>

class Track;
class DataServer : public QObject
{
    Q_OBJECT
public:
    explicit DataServer(QObject *parent = 0);
    ~DataServer();

signals:

public slots:
    void start();
    void stop();
    Track* openTrack(int id);
    Track* createTrack(QString name, int interval);

public:
    QSqlError error();

private:
    QSqlDatabase db;
};

#endif // DATASERVER_H
