#ifndef CATEGORY_HPP
#define CATEGORY_HPP

#include <core/jsonableinterface.hpp>
#include <QList>
#include <QUuid>

class Category : JsonableInterface<Category>
{
public:
    Category();
    
    Category(QString name, QString icon, int position){
        this->name = name;
        this->icon = icon;
        this->position = position;
    }

    QString getName(){return this->name;}
    QString getIconPath(){return this->icon;}
    int getPosition(){return this->position;}
    bool isEnabled(){return this->enabled;}
    QList<QString> getSources(){return this->sources;}

    void setName(const QString & name){this->name = name;}
    void setIconPath(const QString & icon){this->icon = icon;}
    void setEnableStatus(const bool & status){this->enabled = status;}
    void setPosition(const int & value){this->position = value;}
    void setSources(const QList<QString> value){this->sources = value;}

    Category FromJsonObjectToObject(const QJsonObject & json) override;
    QJsonObject ObjectToJsonObject() override;
    
private:
    QString name;
    QString icon;
    bool enabled = true;
    int position;
    QList<QString> sources;
};

#endif