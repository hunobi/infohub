#ifndef SOURCE_HPP
#define SOURCE_HPP

#include <core/jsonableinterface.hpp>

class Source : JsonableInterface<Source>
{
public:
    Source();
    Source(QString name, QString address, QList<QString> categories, bool enabled = true){
        this->name = name;
        this->address = address;
        this->categories = categories;
        this->enabled = enabled;
    }

    QString getName(){return this->name;}
    QString getAddress(){return this->address;}
    QList<QString> getCategories(){return this->categories;}
    bool isEnabled(){return this->enabled;}

    void setName(const QString & name){this->name = name;}
    void setAddress(const QString & address){this->address = address;}
    void setCategories(const QList<QString> & value){this->categories = value;}
    void setEnableStatus(const bool & status){this->enabled = status;}

    Source FromJsonObjectToObject(const QJsonObject & json) override;
    QJsonObject ObjectToJsonObject() override;
    
private:
    QString name;
    QString address;
    QList<QString> categories;
    bool enabled;
};

#endif