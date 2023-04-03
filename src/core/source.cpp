#include <QJsonArray>
#include <core/source.hpp>

/*
    Basic contructor
*/
Source::Source(){}


/*
    Create and return Source object from QJsonObject.

    !!!
    For the future:
        Rethink what to do if data is fudged.
    !!!
*/
Source Source::FromJsonObjectToObject(const QJsonObject & json){
    Source obj;
    
    if(json.contains("name")){
        obj.setName(json.value("name").toString());
    }else{qDebug()<<"[config.json][source]\tName not found";}
    
    if(json.contains("address")){
        obj.setAddress(json.value("address").toString());
    }else{qDebug()<<"[config.json][source]\tAddress not found";}
    
    if(json.contains("enabled")){
        obj.setEnableStatus(json.value("enabled").toBool());
    }else{qDebug()<<"[config.json][source]\tEnabled not found";}
    
    if(json.contains("categories")){
        obj.setCategories(json.value("categories").toVariant().toStringList());
    }else{qDebug()<<"[config.json][source]\tCategory_name not found";}
    
    return obj;    
}

/*
    Convert Source to JsonObject
*/
QJsonObject Source::ObjectToJsonObject(){
    QJsonObject main;
    main.insert("name", getName());
    main.insert("address",getAddress());
    main.insert("categories", QJsonArray::fromStringList(getCategories()));
    main.insert("enabled", isEnabled());
    return main;
}