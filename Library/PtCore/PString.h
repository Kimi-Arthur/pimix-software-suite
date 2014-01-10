#ifndef PSTRING_H
#define PSTRING_H

#include "PtCore_global.h"

namespace Pt {
namespace Core {

class PStringMap;

class PStringPairList;

class PTCORESHARED_EXPORT PString
{
public:
    PString();
    static QString format(QString pattern, const PStringPairList &parameters);
};

class PTCORESHARED_EXPORT PStringMap : public std::map<QString, QString>
{
public:
    PStringMap()
        : std::map<QString, QString>()
    { }

    explicit PStringMap(std::initializer_list<std::pair<const QString, QString>> __l)
        : std::map<QString, QString>(__l)
    {

    }

    static QString serialize(const PStringMap &value);
};

class PTCORESHARED_EXPORT PStringPairList : public QList<QPair<QString, QString>>
{
public:
    PStringPairList()
        : QList<QPair<QString, QString>>()
    { }

    PStringPairList(std::initializer_list<QPair<QString, QString>> __l)
        : QList<QPair<QString, QString>>(__l)
    { }

    PStringPairList(const PStringMap &kvs)
        : PStringPairList()
    {
        for (auto kv : kvs)
            append(QPair<QString, QString>(kv.first, kv.second));
    }
};

}
}

#endif // PSTRING_H
