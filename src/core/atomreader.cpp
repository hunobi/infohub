#include<core/atomreader.hpp>
#include<core/httpclient.hpp>
#include<QFile>
#include<QDir>

/*
    Basic constructor
*/
AtomReader::AtomReader(){}

/*
    Return Atom (ISODate) DateTime as SEC
*/
qint64 AtomReader::convertDateStringToSec(const QString & date){
    QDateTime dateTime = QDateTime::fromString(date, Qt::ISODate);
    if (!dateTime.isValid())
        return -1;
    return dateTime.toSecsSinceEpoch();
}

/*
    Convert all items in XML Element to JsonArray as list of JsonObjects.

    [INFO 1]: Timestamp is equals -1 if format is invalid.
*/
QJsonArray AtomReader::convertToJsonObject(const QDomElement & xml){
    QJsonArray mainArray;
    QDomNodeList items = xml.elementsByTagName("entry");
    for(int i = 0; i < items.count(); i++) {
        QDomNode itemNode = items.at(i);
        if(itemNode.isElement()) {
            QJsonObject obj;

            QDomElement itemElement = itemNode.toElement();
            QString title = removeRssTag(itemElement.elementsByTagName("title").at(0).toElement().text());
            QString description = removeRssTag(itemElement.elementsByTagName("summary").at(0).toElement().text());
            QString url = removeRssTag(itemElement.elementsByTagName("id").at(0).toElement().text());
            qint64 timestamp = -1;
            if(!itemElement.elementsByTagName("updated").isEmpty()){
                timestamp = convertDateStringToSec(removeRssTag(itemElement.elementsByTagName("updated").at(0).toElement().text()));
            }
            QList<QString> tags;
            QDomNodeList tmp_tags = itemElement.elementsByTagName("category");
            for(int j = 0; j<tmp_tags.count(); j++){
                QDomElement catElement = tmp_tags.at(j).toElement();
                QString tmp_str = removeRssTag(catElement.attribute("term"));
                if(!tmp_str.isNull()&&!tmp_str.isEmpty()){
                    tags.append(tmp_str);
                }
            }

            // find image
            QString image_url = findImageUrl(description);
            QString image_path = takeDefaultDayIconPath(timestamp);

            obj.insert("title", title);
            obj.insert("describe", description);
            obj.insert("url", url);
            obj.insert("timestamp", timestamp);
            obj.insert("tags", QJsonArray::fromStringList(tags));
            obj.insert("image_url", image_url);
            obj.insert("image_path", image_path);
            
            mainArray.append(obj);
        }
    }
        
    return mainArray;
}