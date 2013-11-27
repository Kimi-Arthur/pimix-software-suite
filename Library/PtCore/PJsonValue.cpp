#include <QJsonObject>
#include <QJsonArray>
#include "PJsonValue.h"
#include "PSerializer.h"

DeclStart_SerializeFunctions(PJsonValue) =
{
    DeclEntry_SerializeFunction(Normal, PJsonValue, return PJsonValue::serialize(value); )
};

DeclStart_DeserializeFunctions(PJsonValue) =
{
    DeclEntry_DeserializeFunction(Normal, PJsonValue, return PJsonValue::deserialize(data); )
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

QString PJsonValue::serialize(const PJsonValue &value)
{
    QJsonObject o;
    o["o"] = value;
    QString s = QString::fromUtf8(QJsonDocument(o).toJson(QJsonDocument::Compact));
    // s = {"o": x}
    s.chop(1);
    // s = {"o": x
    return s.mid(6);
}

PJsonValue PJsonValue::deserialize(const QString &data)
{
    QJsonObject o = QJsonDocument::fromJson((QSL("{\"o\": ") + data + QSL("}")).toUtf8()).object();
    return PJsonValue(o["o"]);
}

int PJsonValue::toInt(int defaultValue)
{
    if (isDouble())
        return toDouble();
    return defaultValue;
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
