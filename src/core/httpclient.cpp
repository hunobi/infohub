#include <core/httpclient.hpp>
#include <QUrl>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkAccessManager>
#include <QtConcurrent/QtConcurrent>
#include "qeventloop.h"
#include <QDir>
#include <QFile>

bool HttpClient::IsNetworkError(){
    return HttpClient::network_error;
}

int HttpClient::GetStatusCode(){
    return HttpClient::status_code;
}

QString HttpClient::GetContentType(){
    return HttpClient::content_type;
}

QByteArray HttpClient::Download(const QString & url){
    HttpClient::network_error = false;
    QUrl uri(url);
    QNetworkRequest req;
    req.setUrl(uri);
    req.setHeader(QNetworkRequest::KnownHeaders::UserAgentHeader, "WebBrowser 100.0");
    QNetworkAccessManager manager;
    QNetworkReply * reply =  manager.get(req);
    QEventLoop l_event_loop;
    QObject::connect(reply, SIGNAL(finished()), &l_event_loop, SLOT(quit()));
    l_event_loop.exec();
    QVariant contentType = reply->header(QNetworkRequest::ContentTypeHeader);
    if(contentType.isValid()){
        HttpClient::content_type = contentType.toString();
    }
    QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if(status_code.isValid()){
       HttpClient::status_code = status_code.toInt();
       return  reply->readAll();
    }
    HttpClient::network_error = true;
    HttpClient::status_code = -1;
    return NULL;
}

QString HttpClient::DownloadString(const QString & url){
   QByteArray bytes = HttpClient::Download(url);
   return QString(bytes);
}

QJsonDocument HttpClient::DownloadJSON(const QString & url){
    QString data = HttpClient::DownloadString(url);
    return QJsonDocument::fromJson(data.toUtf8());
}

void HttpClient::DownloadFile(const QString & url, const QString & path){
     QByteArray bytes = HttpClient::Download(url);
     QDir dir(path);
     QStringList tmp = dir.absolutePath().split("/");
     tmp.pop_back();
     QString new_path = tmp.join('/');
     dir.setPath(new_path);
     if(!dir.exists()){
         dir.mkpath(dir.absolutePath());
     }
     QFile file(path);
     file.open(QIODevice::WriteOnly);
     file.write(bytes);
     file.close();
}
