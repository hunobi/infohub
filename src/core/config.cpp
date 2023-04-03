#include <core/config.hpp>
#include <QFile>

/*
    Return instance of ConfigSingleton object
*/
ConfigSingleton& ConfigSingleton::getInstance() {
    static ConfigSingleton instance;
    return instance;
}

/*
    Save Config object to config.json.
*/
void ConfigSingleton::Save()
{
    QFile file(ConfigSingleton::config_path);
    if (file.open(QIODevice::WriteOnly)) {
        QJsonDocument doc = ConfigSingleton::cfg.createJsonDocument();
        if(!doc.isNull()){
            file.write(doc.toJson(QJsonDocument::Indented));
        }
        file.close();
    }
}

/*
    This way is using for save default config.json
    This method will execute if config.json file does not exist.
*/
void ConfigSingleton::Save(const QByteArray & data){
    QFile file(ConfigSingleton::config_path);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(data);
        file.close();
    }
}


/*
    Remove existing ConfigSingleton.json
*/
void ConfigSingleton::Remove(){
    QFile file(ConfigSingleton::config_path);
    if(file.exists()){
        file.remove();
    }
}


/*
    Load ConfigSingleton.json file. 
    If file not exist or existing file is not valid, create default ConfigSingleton.json.
*/
void ConfigSingleton::Load()
{
    QFile file(ConfigSingleton::config_path);
    if(!file.exists()){
        QFile defaultConfigSingleton(":/config.json");
        defaultConfigSingleton.open(QFile::ReadOnly);
        QByteArray data = defaultConfigSingleton.readAll();
        defaultConfigSingleton.close();
        Save(data);
    }
    if (file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        file.close();
        if(doc.isNull()){
            Remove();
            Load();
        }
        // init Config object from json document
        ConfigSingleton::cfg = ConfigSingleton::cfg.FromJsonObjectToObject(doc.object());
    }
    /*
***********************
        What to do when config.json can not open?
***********************
    */
}

/*
    Return reference to Config object.
*/
Config& ConfigSingleton::GetConfig()
{
    return this->cfg;
}


/*
    Init ConfigSingleton and Load config.json to memory
*/
ConfigSingleton::ConfigSingleton() {
    Load();
}

/*

===============================================================

*/


/*
    Constructor of Config object
*/
Config::Config(){}


/*
    Create Config object from JsonObject
*/
Config Config::FromJsonObjectToObject(const QJsonObject & json){
    // load settings
    if(json.contains("settings")){
        this->settings = settings.FromJsonObjectToObject(json.value("settings").toObject());
    }else{qDebug()<<"[config.json]\tSettings not found";}
    
    // load categories
    if(json.contains("categories")){
        categories.clear();
        QJsonObject obj = json.value("categories").toObject();
        if(!obj.isEmpty()){
            for(auto key : obj.keys()){
                Category value;
                value = value.FromJsonObjectToObject(obj.value(key).toObject());
                this->categories.insert(key, value);
            }
        }
    }else{qDebug()<<"[config.json]\tCategories not found";}
    
    // load sources
    if(json.contains("sources")){
        sources.clear();
        QJsonObject obj = json.value("sources").toObject();
        if(!obj.isEmpty()){
            for(auto key : obj.keys()){
                Source value;
                value = value.FromJsonObjectToObject(obj.value(key).toObject());
                this->sources.insert(key, value);
            }
        }
    }else{qDebug()<<"[config.json]\tSources not found";}
    
    // load news
    if(json.contains("news")){
        news.clear();
        QJsonObject obj = json.value("news").toObject();
        if(!obj.isEmpty()){
            for(auto key : obj.keys()){
                News value;
                value = value.FromJsonObjectToObject(obj.value(key).toObject());
                this->news.insert(key, value);
            }
        }
    }else{qDebug()<<"[config.json]\tNews not found";}
    
    return *this;
}


/*
    Method will convert Map<key,value> to JsonObject.

    Warning: Value must be Jsonable. (JsonableInterface)
*/
template<typename T>
QJsonObject Config::mapToJson(const QMap<QString, T>& map)
{
    QJsonObject result;
    for (auto key : map.keys()) {
        QJsonObject tmp = map.value(key).ObjectToJsonObject();
        result.insert(key, tmp);
    }
    return result;
}

/*
    Create Main JsonObject from Config object
*/
QJsonObject Config::ObjectToJsonObject(){
    QJsonObject main;
    // add settings
    main.insert("settings", settings.ObjectToJsonObject());
    // add categories
    main.insert("categories", mapToJson(categories));
    // add sources
    main.insert("sources", mapToJson(sources));
    // add news
    main.insert("news", mapToJson(news));
    // return main json object
    return main;
}


/*
    Create JsonDocument from Config object
*/
QJsonDocument Config::createJsonDocument(){
    QJsonDocument doc;
    doc.setObject(Config::ObjectToJsonObject());
    return doc;
}
