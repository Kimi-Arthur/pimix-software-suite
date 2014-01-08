#ifndef PSTRING_H
#define PSTRING_H

#include "PtCore_global.h"

namespace Pt {
namespace Core {

class PStringMap;

class PTCORESHARED_EXPORT PString
{
public:
    PString();
    static QString format(QString pattern, PStringMap parameters);
};

class PTCORESHARED_EXPORT PStringMap : public std::map<QString, QString>
{
public:
    PStringMap()
        : std::map<QString, QString>()
    { }

    PStringMap(std::initializer_list<std::pair<const QString, QString>> __l)
        : std::map<QString, QString>(__l)
    {

    }

    QMap<QString, QString> toQStringMap() const
    {
        return QMap<QString, QString>(*this);
    }

    static QString serialize(const PStringMap &value);
};

}
}

#endif // PSTRING_H
