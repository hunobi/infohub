#ifndef JSONABLEINTERFACE_HPP
#define JSONABLEINTERFACE_HPP

#include <QString>
#include <QJsonObject>

template<class T>
class JsonableInterface
{
public:
    virtual QJsonObject ObjectToJsonObject() = 0;
    virtual T FromJsonObjectToObject(const QJsonObject & json) = 0;
};

#endif