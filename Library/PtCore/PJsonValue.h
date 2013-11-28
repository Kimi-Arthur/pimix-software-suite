#ifndef PJSONVALUE_H
#define PJSONVALUE_H

#include "PtCore_global.h"
#include <QJsonDocument>
#include <QJsonValue>

class PJsonValueRef;

class PTCORESHARED_EXPORT PJsonValue : public QJsonValue
{
public:
    PJsonValue(Type type = Null) : QJsonValue(type) {}
    PJsonValue(QJsonValue val) : QJsonValue(val) {}
    PJsonValue operator [](QString key) const;
    PJsonValueRef operator [](QString key);

    PJsonValue operator [](int index) const;
    PJsonValueRef operator [](int index);
    QJsonDocument toDocument() const;
    static QString serialize(const PJsonValue &value);
    static PJsonValue deserialize(const QString &data);
    int toInt(int defaultValue = 0);
};

class PTCORESHARED_EXPORT PJsonValueRef : public QJsonValueRef
{
public:
    PJsonValueRef(QJsonValueRef ref) : QJsonValueRef(ref) {}
    PJsonValue operator [](QString key) const;
    PJsonValueRef operator [](QString key);

    PJsonValue operator [](int index) const;
    PJsonValueRef operator [](int index);
};


#endif // PJSONVALUE_H
