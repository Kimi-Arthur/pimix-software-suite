#include <QJsonValue>
#include <QJsonObject>
#include <QJsonArray>
#include "PJsonValue.h"

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
