#include <core/utils.hpp>
#include <core/rssreader.hpp>
#include <core/atomreader.hpp>
#include <QFile>
#include <QDir>
#include <QDomDocument>
#include <QDateTime>
#include <QCryptographicHash>
#include <QRegularExpression>
#include <QTextDocument>
#include <QMimeDatabase>
#include <core/httpclient.hpp>

QString calculateSHA256(const QJsonObject& json)
{
    QCryptographicHash hash(QCryptographicHash::Sha256);
    hash.addData(QJsonDocument(json).toJson(QJsonDocument::Compact));
    return QString(hash.result().toHex());
}

QString calculateSHA256(const QByteArray& bytes)
{
    QCryptographicHash hash(QCryptographicHash::Sha256);
    hash.addData(bytes);
    return QString(hash.result().toHex());
}

QString removeRssTag(const QString& input){
    QString text = input;
    text.replace("<![CDATA[","");
    text.replace("]]>","");
    return text;
}

QString removeHtmlTags(const QString& input) {
    QString text = input;
    // Usuń wszystkie tagi HTML wraz z ich zawartością
    text.remove(QRegularExpression("<[^>]*>"));
    // Zamień znaki specjalne na odpowiedniki UTF8
    QTextDocument document;
    document.setHtml(text);
    return document.toPlainText();
}

QString findImageUrl(const QString& description)
{
    QRegularExpression regex("<img[^>]*src=\"([^\"]+)\"");
    QRegularExpressionMatch match = regex.match(description);
    if (match.hasMatch()) {
        return match.captured(1);
    } else {
        return QString();
    }
}

QString takeDefaultDayIconPath(const qint64 & timestamp){
    int dayOfWeek;
    if(timestamp>-1){
        dayOfWeek = QDateTime::fromSecsSinceEpoch(timestamp).date().dayOfWeek();
    }else{
        dayOfWeek = QDateTime::fromSecsSinceEpoch(getTimeSec()).date().dayOfWeek();
    }
    return ":/icons/day_"+QString::number(dayOfWeek);
}

QString downloadImageToCacheFromUrl(const QString & url, const QString & news_hash){
    HttpClient agent;
    QByteArray bytes = agent.Download(url);
    if(!agent.IsNetworkError() && agent.GetStatusCode() == 200){
        int numOfMonth = QDateTime::fromSecsSinceEpoch(getTimeSec()).date().month();
        QString img_path = "cache/"+QString::number(numOfMonth)+"/"+news_hash;
        QFile img(img_path);
        if(!img.exists()){ // if exist do nothing
            QDir dir;
            dir.mkpath("cache/"+QString::number(numOfMonth));
            img.open(QFile::WriteOnly);
            img.write(bytes);
            img.close();
        }
        return img_path;
    }
    return QString();
}

QString mimeType( const QString &filePath ){ return QMimeDatabase().mimeTypeForFile( filePath ).name(); }

qint64 getTimeMS(){return QDateTime::currentMSecsSinceEpoch();}
qint64 getTimeSec(){return QDateTime::currentSecsSinceEpoch();}

/*
    Return QJsonArray of QJsonObject. Simple json object structure: 
    {
        title
        describe
        url
        timestamp
        tags
        image_url
        image_path
    }
*/
QJsonArray getNewsArrayFromSource(const QString & url){
    HttpClient agent;
    QString response = agent.DownloadString(url);
    if(!agent.IsNetworkError() && agent.GetStatusCode() == 200){
        QDomDocument xml;
        if(xml.setContent(response)){
            if(xml.documentElement().tagName() == "rss"){
                RssReader rss;
                return rss.convertToJsonObject(xml.documentElement());
            }
            else if(xml.documentElement().tagName() == "feed"){
                AtomReader atom;
                return atom.convertToJsonObject(xml.documentElement());
            }else{
                throw std::runtime_error("Unknown news format. App can read only RSS and Atom news.");
            }
        }
    }

    QJsonArray arr;
    return arr;
    //throw std::runtime_error(url+"\t");
}

QString secondsToDateTimeString(qint64 seconds) {
    QDateTime dateTime = QDateTime::fromSecsSinceEpoch(seconds);
    return dateTime.toString("yyyy-MM-dd hh:mm");
}