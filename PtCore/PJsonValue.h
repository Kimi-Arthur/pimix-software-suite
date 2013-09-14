#ifndef PJSONVALUE_H
#define PJSONVALUE_H

#include "ptcore_global.h"
#include <QJsonValue>

class PJsonValueRef;

class PTCORESHARED_EXPORT PJsonValue : public QJsonValue
{
public:
    PJsonValue(QJsonValue val) : QJsonValue(val) {}
    PJsonValue operator [](QString key) const;
    PJsonValueRef operator [](QString key);

    PJsonValue operator [](int index) const;
    PJsonValueRef operator [](int index);
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
