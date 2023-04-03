#ifndef ATOMREADER_HPP
#define ATOMREADER_HPP

#include<core/readerinterface.hpp>

class AtomReader : ReaderInterface{
public:
    AtomReader();
    QJsonArray convertToJsonObject(const QDomElement & xml) override;
private:
    qint64 convertDateStringToSec(const QString & date);
};

#endif