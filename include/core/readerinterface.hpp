#ifndef READERINTERFACE_HPP
#define READERINTERFACE_HPP

#include <core/news.hpp>
#include <core/utils.hpp>
#include <QDomDocument>
#include <QDomNode>
#include <QDomNodeList>
#include <QDomElement>
#include <QJsonObject>
#include <QJsonArray>

class ReaderInterface
{
public:
    virtual QJsonArray convertToJsonObject(const QDomElement & xml) = 0;
};

#endif