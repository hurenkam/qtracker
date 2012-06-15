#ifndef GPXFILE_H
#define GPXFILE_H

#include <QObject>
#include <QFile>
#include <QVariant>
#include <QString>
#include <QList>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

class XmlIO
{
protected:
    QXmlStreamReader reader;
    QXmlStreamWriter writer;
    QXmlStreamReader::TokenType tt;

public:
    QString ReadElementString()            { return reader.readElementText(); }
    double  ReadElementDouble()            { return reader.readElementText().toDouble(); }
    QString ReadAttributeString(QString a) { return reader.attributes().value(a).toString(); }
    double  ReadAttributeDouble(QString a) { return reader.attributes().value(a).toString().toDouble(); }
    double  ReadAttributeInt(QString a)    { return reader.attributes().value(a).toString().toInt(); }

};

class GpxFile : public QObject, public XmlIO
{
    Q_OBJECT
    Q_PROPERTY(QVariant fileName READ getFileName WRITE setFileName NOTIFY fileNameChanged)

public:
    explicit GpxFile(QObject *parent = 0);
    void     setFileName(QVariant value);
    QVariant getFileName() { return filename; }

    Q_INVOKABLE void reset();
    Q_INVOKABLE void parseGpx();
    Q_INVOKABLE void parseWayPoint();
    Q_INVOKABLE void parseRoute();
    Q_INVOKABLE void parseTrack();
    Q_INVOKABLE void parseResolution();
    Q_INVOKABLE void parseRefpoint();

    Q_INVOKABLE void openGpx();
    Q_INVOKABLE void closeGpx();
    Q_INVOKABLE void writePoint(const QString& tag, QVariantMap pt);
    Q_INVOKABLE void writeWayPoint(QVariantMap wpt);
    Q_INVOKABLE void openRoute(QVariantMap rte);
    Q_INVOKABLE void closeRoute();
    Q_INVOKABLE void writeRoutePoint(QVariantMap rtept);
    Q_INVOKABLE void openTrack(QVariantMap trk);
    Q_INVOKABLE void closeTrack();
    Q_INVOKABLE void writeTrackPoint(QVariantMap trkpt);

signals:
    void fileNameChanged(const QString& fileName);
    void wayPoint(QVariant wpt);
    void routeFound();
    void routeName(const QString& routeName);
    void routePoint(QVariant rtept);
    void trackFound();
    void trackName(const QString& trackName);
    void trackPoint(QVariant trkpt);
    void mapFound(QVariant map);
    void resolution(QVariant resolution);
    void refPoint(QVariant refpt);

public slots:

private:
    QVariant parsePoint(QString tag);

    QFile file;
    QString filename;
};

#endif // GPXFILE_H
