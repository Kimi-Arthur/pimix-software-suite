#include <QJsonObject>
#include <QJsonArray>
#include "PJsonValue.h"
#include "PSerializer.h"

DeclStart_SerializeFunctions(PJsonValue) =
{
    Decl_SerializeFunctionEntry(Normal, PJsonValue, return QString::fromUtf8(value.toDocument().toJson(QJsonDocument::JsonFormat::Compact)); )
};

PJsonValue PJsonValue::operator [](QString key) const
{
    return PJsonValue(this->toObject()[key]);
}

PJsonValueRef PJsonValue::operator [](QString key)
{
    return PJsonValueRef(this->toObject()[key]);
}

PJsonValue PJsonValue::operator [](int index) const
{
    return PJsonValue(this->toArray()[index]);
}

PJsonValueRef PJsonValue::operator [](int index)
{
    return PJsonValueRef(this->toArray()[index]);
}

QJsonDocument PJsonValue::toDocument() const
{
    switch (type()) {
    case Type::Array:
        return QJsonDocument(this->toArray());
    case Type::Object:
        return QJsonDocument(this->toObject());
    default:
        return QJsonDocument();
    }
}


PJsonValue PJsonValueRef::operator [](QString key) const
{
    return PJsonValue(this->toObject()[key]);
}

PJsonValueRef PJsonValueRef::operator [](QString key)
{
    return PJsonValueRef(this->toObject()[key]);
}

PJsonValue PJsonValueRef::operator [](int index) const
{
    return PJsonValue(this->toArray()[index]);
}

PJsonValueRef PJsonValueRef::operator [](int index)
{
    return PJsonValueRef(this->toArray()[index]);
}
