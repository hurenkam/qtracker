#ifndef DATAPROVIDER_H
#define DATAPROVIDER_H

#include <QLocalServer>
#include <QLocalSocket>

class DataProvider: public QObject
{
    Q_OBJECT

public:
    DataProvider(QString name, QObject* parent=0)
        : QObject(parent)
    {
        server = new QLocalServer(this);
        if (!server->listen(name)) {
            qDebug() << "DataProvider::DataProvider(): unable to start " << name << " server!";
        }
        connect(server, SIGNAL(newConnection()), this, SLOT(_sendData()));
    }

private slots:
    void _sendData()
    {
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_0);
        out << (quint16)0;
        sendData(out);
        out.device()->seek(0);
        out << (quint16)(block.size() - sizeof(quint16));

        QLocalSocket *clientConnection = server->nextPendingConnection();
        connect(clientConnection, SIGNAL(disconnected()),
                clientConnection, SLOT(deleteLater()));

        clientConnection->write(block);
        clientConnection->flush();
        clientConnection->disconnectFromServer();
    }

protected:
    virtual bool sendData(QDataStream& out)=0;

private:
    QLocalServer *server;
};

#endif // DATAPROVIDER_H
