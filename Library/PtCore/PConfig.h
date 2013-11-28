#ifndef PCONFIG_H
#define PCONFIG_H

#include "PtCore_global.h"

#include "PDataModel.h"

namespace Pt {
namespace Core {

class PTCORESHARED_EXPORT PConfig
{
protected:
    QString moduleName;
    QStringList saerchPathList = {".", "~", "/etc"};
    int currentPathIndex = 0;
public:
    PConfig();
};

}
}
#endif // PCONFIG_H
