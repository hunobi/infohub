#ifndef HTTPCLIENT_HPP
#define HTTPCLIENT_HPP

#include <QJsonDocument>
#include <QString>

class HttpClient{
public:
    HttpClient(){
        network_error = false;
    }
    void DownloadFile(const QString & url, const QString & path);
    QJsonDocument DownloadJSON(const QString & url);
    QString DownloadString(const QString & url);
    QByteArray Download(const QString & url);
    QString GetContentType();
    int GetStatusCode();
    bool IsNetworkError();
private:
    QString content_type;
    int status_code;
    bool network_error;
};

#endif