#ifndef NEWS_HPP
#define NEWS_HPP

#include <core/jsonableinterface.hpp>
#include <QUuid>
#include <QDomDocument>
#include <QJsonArray>

class News : JsonableInterface<News>
{
public:
    News();
    News(const QString & source_name, const QJsonObject & json);

    News(const QString & title, const QString & url, const QString & describe, const QString & image_path, 
         const QList<QString> & categories, const QList<QString> & tags, const QString & source_name, 
         const qint64 & timestamp, const bool & read, const bool & saved, const bool & hidden)
    {
        this->title = title;
        this->url = url;
        this->describe = describe;
        this->image_path = image_path;
        this->categories = categories;
        this->tags = tags;
        this->source_name = source_name;
        this->timestamp = timestamp;
        this->read = read;
        this->saved = saved;
        this->hidden = hidden;
    }

    bool operator==(const News & otherNews) const;
    
    QString getHash()const{return this->hash;}
    QList<QString> getCategories(){return this->categories;}
    QList<QString> getTags(){return this->tags;}
    QString getSourceName(){return this->source_name;}
    QString getTitle(){return this->title;}
    QString getUrl(){return this->url;}
    QString getDescribe(){return this->describe;}
    QString getImagePath(){return this->image_path;}
    qint64 getTimestamp(){return this->timestamp;}
    bool isRead(){return this->read;}
    bool isSaved(){return this->saved;}
    bool isHidden(){return this->hidden;}

    void setHash(const QString & value){this->hash=value;}
    void setCategories(const QList<QString> & value){this->categories = value;}
    void setTags(const QList<QString> & value){this->tags = value;}
    void setSourceName(const QString & value){this->source_name = value;}
    void setTitle(const QString & value){this->title = value;}
    void setUrl(const QString & value){this->url = value;}
    void setDescribe(const QString & value){this->describe = value;}
    void setImagePath(const QString & value){this->image_path = value;}
    void setTimestamp(const qint64 & value){this->timestamp = value;}
    void setReadStatus(const bool & status){this->read = status;}
    void setSavedStatus(const bool & status){this->saved = status;}
    void setHiddenStatus(const bool & status){this->hidden = status;}

    QString computeHash();
    News FromJsonObjectToObject(const QJsonObject & json) override;
    QJsonObject ObjectToJsonObject() override;
    

private:
    QString hash;
    QList<QString> categories;
    QList<QString> tags;
    QString source_name;
    QString title;
    QString url;
    QString describe;
    QString image_path;
    qint64 timestamp;
    bool read = false;
    bool saved = false;
    bool hidden = false;
};

#endif