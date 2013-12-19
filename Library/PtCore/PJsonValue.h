#ifndef PJSONVALUE_H
#define PJSONVALUE_H

#include "PtCore_global.h"
#include "PString.h"
#include <QJsonDocument>
#include <QJsonValue>

namespace Pt {
namespace Core {
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
    PStringMap toMap() const;
private:
    QString normalizeSlash(QString rawKey) const;
};

class PTCORESHARED_EXPORT PJsonValueRef : public QJsonValueRef
{
public:
    PJsonValueRef(QJsonValueRef ref) : QJsonValueRef(ref) {}

    PJsonValueRef &operator = (const PJsonValue &val);
    PJsonValueRef &operator = (const PJsonValueRef &val);

    PJsonValue operator [](QString key) const;
    PJsonValueRef operator [](QString key);

    PJsonValue operator [](int index) const;
    PJsonValueRef operator [](int index);
};


}
}

#endif // PJSONVALUE_H
