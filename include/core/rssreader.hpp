#ifndef RSSREADER_HPP
#define RSSREADER_HPP

#include<core/readerinterface.hpp>

/*
    This Reader use RSS 2.0
*/
class RssReader : ReaderInterface{
public:
    RssReader();
    QJsonArray convertToJsonObject(const QDomElement & xml) override;
private:
    qint64 convertDateStringToSec(const QString & date);
};

#endif