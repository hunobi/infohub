#ifndef UTILS_HPP
#define UTILS_HPP

#include<QString>
#include<QJsonDocument>
#include<QJsonObject>
#include<QJsonArray>

QString calculateSHA256(const QJsonObject& json);
QString calculateSHA256(const QByteArray& bytes);

QString removeHtmlTags(const QString& input);
QString removeRssTag(const QString& input);

QString mimeType( const QString &filePath );

QString findImageUrl(const QString& description);
QString takeDefaultDayIconPath(const qint64 & timestamp);

QJsonArray getNewsArrayFromSource(const QString & url);

/*
    Return Path to Cache image file.
*/
QString downloadImageToCacheFromUrl(const QString & url, const QString & news_hash);

qint64 getTimeMS();
qint64 getTimeSec();

QString secondsToDateTimeString(qint64 seconds);

#endif