#ifndef DISTANCEPROVIDER_H
#define DISTANCEPROVIDER_H

class DistanceProvider: public DataProvider
{
public:
    DistanceProvider()
        : DataProvider("distance")
        , current(20)
    {}
    virtual bool sendData(QDataStream& out)
    { out << current; return true; }

private:
    double current;
};

#endif // DISTANCEPROVIDER_H
