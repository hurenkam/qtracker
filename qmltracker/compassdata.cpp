#include <QtCore>
#include <QtDeclarative>

class CompassData: public QObject
{
    Q_OBJECT
    Q_PROPERTY(int angle READ angle WRITE setAngle NOTIFY angleChanged)
    Q_PROPERTY(int accuracy READ accuracy WRITE setAccuracy NOTIFY accuracyChanged)

private:
    int _angle;
    int _accuracy;

public:
    int  angle()                   { return _angle; }
    void setAngle(int angle)       { _angle = angle; }
    int  accuracy()                { return _accuracy; }
    void setAccuracy(int accuracy) { _accuracy = accuracy; }

signals:
    void angleChanged();
    void accuracyChanged();
};
