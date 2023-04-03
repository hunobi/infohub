#include <core/news.hpp>
#include <core/utils.hpp>
#include <core/config.hpp>

/*
    Basic contructor
*/
News::News(){};

/*
    Convert QJsonObject to News object. This method includes field validation.
*/
News News::FromJsonObjectToObject(const QJsonObject & json)
{
    if (json.isEmpty())
        throw std::invalid_argument("[config.json][news]\tInput JSON object is empty.");

    News news;
    if (!json.contains("hash") || !json["hash"].isString())
        throw std::invalid_argument("[config.json][news]\tInvalid input JSON object. Missing or invalid 'hash' field.");
    news.setHash(json["hash"].toString());

    if (!json.contains("categories") || !json["categories"].isArray())
        throw std::invalid_argument("[config.json][news]\tInvalid input JSON object. Missing or invalid 'categories' field.");
    QList<QString> categories;
    QJsonArray categoriesArray = json["categories"].toArray();
    for (auto category : categoriesArray) {
        if (!category.isString())
            throw std::invalid_argument("[config.json][news][categories]\tInvalid input JSON object. Invalid 'categories' field item.");
        categories.append(category.toString());
    }
    news.setCategories(categories);

    if (!json.contains("tags") || !json["tags"].isArray())
        throw std::invalid_argument("[config.json][news]\tInvalid input JSON object. Missing or invalid 'tags' field.");
    QList<QString> tags;
    QJsonArray tagsArray = json["tags"].toArray();
    for (auto tag : tagsArray) {
        if (!tag.isString())
            throw std::invalid_argument("[config.json][news][tags]\tInvalid input JSON object. Invalid 'tags' field item.");
        tags.append(tag.toString());
    }
    news.setTags(tags);

    if (!json.contains("source_name") || !json["source_name"].isString())
        throw std::invalid_argument("[config.json][news]\tInvalid input JSON object. Missing or invalid 'source_name' field.");
    news.setSourceName(json["source_name"].toString());

    if (!json.contains("title") || !json["title"].isString())
        throw std::invalid_argument("[config.json][news]\tInvalid input JSON object. Missing or invalid 'title' field.");
    news.setTitle(json["title"].toString());

    if (!json.contains("url") || !json["url"].isString())
        throw std::invalid_argument("[config.json][news]\tInvalid input JSON object. Missing or invalid 'url' field.");
    news.setUrl(json["url"].toString());

    if (!json.contains("describe") || !json["describe"].isString())
        throw std::invalid_argument("[config.json][news]\tInvalid input JSON object. Missing or invalid 'describe' field.");
    news.setDescribe(json["describe"].toString());

    if (!json.contains("image_path") || !json["image_path"].isString())
        throw std::invalid_argument("[config.json][news]\tInvalid input JSON object. Missing or invalid 'image_path' field.");
    news.setImagePath(json["image_path"].toString());

    if (!json.contains("timestamp") || !json["timestamp"].isDouble())
        throw std::invalid_argument("[config.json][news]\tInvalid input JSON object. Missing or invalid 'timestamp' field.");
    news.setTimestamp(json["timestamp"].toInteger());

    if (!json.contains("read") || !json["read"].isBool())
        throw std::invalid_argument("[config.json][news]\tInvalid input JSON object. Missing or invalid 'read' field.");
    news.setReadStatus(json["read"].toBool());

    if (!json.contains("saved") || !json["saved"].isBool())
        throw std::invalid_argument("[config.json][news]\tInvalid input JSON object. Missing or invalid 'saved' field.");
    news.setSavedStatus(json["saved"].toBool());

    if (!json.contains("hidden") || !json["hidden"].isBool())
        throw std::invalid_argument("[config.json][news]\tInvalid input JSON object. Missing or invalid 'hidden' field.");
    news.setHiddenStatus(json["hidden"].toBool());

    return news;
}


/*
    Create QJsonObject from News object
*/
QJsonObject News::ObjectToJsonObject(){
    QJsonObject data;
    data.insert("hash", getHash());
    data.insert("source_name", getSourceName());
    data.insert("title",getTitle());
    data.insert("url",getUrl());
    data.insert("describe",getDescribe());
    data.insert("image_path",getImagePath());
    data.insert("timestamp",getTimestamp());
    data.insert("read",isRead());
    data.insert("saved",isSaved());
    data.insert("hidden",isHidden());
    data.insert("categories",QJsonArray::fromStringList(getCategories()));
    data.insert("tags",QJsonArray::fromStringList(getTags()));
    return data;
}

/*
    Operator whose job is to compare two objects.
*/
bool News::operator==(const News & otherNews) const{
    return this->getHash() == otherNews.getHash();
}

/*
    Compute hash of News object.
    This method sets the hash field and returns the hash string.
*/
QString News::computeHash(){
    QJsonObject body;
    //body.insert("source_name", this->getSourceName());
    //body.insert("title",this->getTitle());
    body.insert("url",this->getUrl());
    //body.insert("describe",this->getDescribe());
    //body.insert("tags", QJsonArray::fromStringList(this->getTags()));
    QString hash = calculateSHA256(body);
    this->hash = hash;
    return this->getHash();
}

/*
    This constructor is for create News object from JsonObject during reading
    JsonArray from XML (RSS/Atom)
*/
News::News(const QString & source_name, const QJsonObject & json)
{   
    // set source name
    this->source_name = source_name;
    // set values from json
    this->title = json.value("title").toString();
    this->describe = json.value("describe").toString();
    this->url = json.value("url").toString();
    this->timestamp = json.value("timestamp").toInteger();
    this->tags = json.value("tags").toVariant().toStringList();
    this->image_path = json.value("image_path").toString();
    // default
    this->read = false;
    this->saved = false;
    this->hidden = false;
    // compute and set hash
    this->computeHash();
    // set categories: 
    // [info]: News inherits categories afrom its parents
    auto sources = ConfigSingleton::getInstance().GetConfig().getSources();
    Source src = sources.find(source_name).value();
    this->categories = src.getCategories();
}
