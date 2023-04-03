#include<core/rssreader.hpp>
#include<core/httpclient.hpp>
#include<QFile>
#include<QDir>

RssReader::RssReader(){}

/*
    Return RSS DateTime as SEC
*/
qint64 RssReader::convertDateStringToSec(const QString & date){
    QStringList dateParts = date.split(' ');
    QString dayStr = dateParts[1];
    QString monthStr = dateParts[2];
    QString yearStr = dateParts[3];
    QString timeStr = dateParts[4];
    QString offsetStr = dateParts[5];

    if (offsetStr.contains("GMT")) {
        offsetStr.replace("GMT","+0000");
    }

    // Convert day
    int day = dayStr.toInt();
    QString dayStrIso = (day < 10) ? "0" + QString::number(day) : QString::number(day);

    // Convert month
    QDate tmp_date = QDate::fromString(monthStr, "MMM");
    QString monthStrIso = QString::number(tmp_date.month());
    if (tmp_date.month() < 10) {
        monthStrIso = "0" + monthStrIso;
    }

    // Convert year
    QString yearStrIso = yearStr;

    // Convert time
    QString timeStrIso = timeStr;

    // iso format builder
    QString isoDate = yearStrIso + "-" + monthStrIso + "-" + dayStrIso + "T" + timeStrIso + offsetStr;
    
    // convert iso to QDateTime object
    QDateTime dateTime = QDateTime::fromString(isoDate, Qt::ISODate);
    
    if (!dateTime.isValid())
        return -1; 
    return dateTime.toSecsSinceEpoch();
}

/*
    Convert all items in XML Element to JsonArray as list of JsonObjects.

    [INFO 1]: Timestamp is equals -1 if format is invalid.
*/
QJsonArray RssReader::convertToJsonObject(const QDomElement & xml)
{
    QJsonArray mainArray;

    QDomNodeList items = xml.elementsByTagName("item");
    for(int i = 0; i < items.count(); i++) {
        QDomNode itemNode = items.at(i);
        if(itemNode.isElement()) {
            QJsonObject obj;
            QDomElement itemElement = itemNode.toElement();
            QString title = removeRssTag(itemElement.elementsByTagName("title").at(0).toElement().text());
            QString description = removeRssTag(itemElement.elementsByTagName("description").at(0).toElement().text());
            QString url = removeRssTag(itemElement.elementsByTagName("link").at(0).toElement().text());
            qint64 timestamp = -1;
            if(!itemElement.elementsByTagName("pubDate").isEmpty()){
                timestamp = convertDateStringToSec(removeRssTag(itemElement.elementsByTagName("pubDate").at(0).toElement().text()));
            }
            QList<QString> tags;
            QDomNodeList tmp_tags = itemElement.elementsByTagName("category");
            for(int j = 0; j<tmp_tags.count(); j++){
                QDomElement catElement = tmp_tags.at(j).toElement();
                QString tmp_str = removeRssTag(catElement.text());
                if(!tmp_str.isNull()&&!tmp_str.isEmpty()){
                    tags.append(tmp_str);
                }
            }
            // find image
            QString image_url = "";
            QString image_path = takeDefaultDayIconPath(timestamp);
            QDomNodeList multimedia = itemElement.elementsByTagName("enclosure");
            for(int j = 0; j < multimedia.count(); j++){
                QDomNode mItem = multimedia.at(j);
                if(mItem.isElement()){
                    QDomElement tmpItem = mItem.toElement();
                    if(tmpItem.attributes().contains("type") && tmpItem.attributes().contains("url") && tmpItem.attributeNode("type").value().contains("image")){
                        image_url = tmpItem.attributeNode("url").value();
                        break;
                    }
                }
            }

            if(image_url.isEmpty()){image_url=findImageUrl(description);}

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