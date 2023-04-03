#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <core/jsonableinterface.hpp>
#include <core/settings.hpp>
#include <core/category.hpp>
#include <core/source.hpp>
#include <core/news.hpp>
#include <QJsonDocument>
#include <QJsonObject>


class Config : JsonableInterface<Config>{
public:
    Config();
    QJsonObject ObjectToJsonObject() override;
    Config FromJsonObjectToObject(const QJsonObject & json) override;
    QJsonDocument createJsonDocument();

    Settings & getSettings(){return this->settings;}
    QMap<QString, Category> & getCategories(){return this->categories;}
    QMap<QString, Source> & getSources(){return this->sources;}
    QMap<QString, News> & getNews(){return this->news;}

private:
    template<typename T>
    QJsonObject mapToJson(const QMap<QString, T> & map);
    Settings settings;
    QMap<QString, Category> categories;
    QMap<QString, Source> sources;
    QMap<QString, News> news;
};


class ConfigSingleton
{
public:
    static ConfigSingleton& getInstance();
    void Save();
    void Save(const QByteArray & data);
    void Load();
    Config & GetConfig();

private:
    ConfigSingleton();
    ConfigSingleton(ConfigSingleton const&) = delete;
    void operator=(ConfigSingleton const&) = delete;
    void Remove();
    Config cfg;
    QString config_path = "config.json";
};

#endif // CONFIG_H
