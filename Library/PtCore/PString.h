#ifndef PSTRING_H
#define PSTRING_H

#include "PtCore_global.h"

namespace Pt {
namespace Core {

class PTCORESHARED_EXPORT PString
{
public:
    PString();
    static QString format(QString pattern, std::map<QString, QString> parameters);
};

}
}

#endif // PSTRING_H
