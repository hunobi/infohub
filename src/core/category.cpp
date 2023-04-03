#include <core/category.hpp>
#include <QJsonArray>

/*
    Basic constructor
*/
Category::Category(){}

/*
    Create and return Category object from QJsonObject.

    !!!
    For the future:
        Rethink what to do if data is fudged.
    !!!
*/
Category Category::FromJsonObjectToObject(const QJsonObject & json){
    Category obj;
    
    if(json.contains("name")){
        obj.setName(json.value("name").toString());
    }else{qDebug()<<"[config.json][category]\tName not found";}
    
    if(json.contains("icon")){
        obj.setIconPath(json.value("icon").toString());
    }else{qDebug()<<"[config.json][category]\tIcon not found";}
    
    if(json.contains("enabled")){
        obj.setEnableStatus(json.value("enabled").toBool());
    }else{qDebug()<<"[config.json][category]\tEnabled not found";}
    
    if(json.contains("position")){
        obj.setPosition(json.value("position").toInt());
    }else{qDebug()<<"[config.json][category]\tPosition not found";}
    
    if(json.contains("sources")){
        QList<QString> tmp;
        QJsonArray arr = json.value("sources").toArray();
        for(auto value : arr){
            tmp.append(value.toString());
        }
        obj.setSources(tmp);
    }else{qDebug()<<"[config.json][category]\tSources not found";}

    return obj;
}


/*
    Convert Category object to JsonObject
*/
QJsonObject Category::ObjectToJsonObject(){
    QJsonObject main;
    main.insert("name", getName());
    main.insert("icon",getIconPath());
    main.insert("enabled", isEnabled());
    main.insert("position", getPosition());
    QJsonArray arr;
    for(QString src: sources){
        arr.append(src);
    }
    main.insert("sources", arr);
    return main;
}