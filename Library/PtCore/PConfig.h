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
    std::map<QString, PJsonValue> data;
public:
    PDataModel(const PJsonValue &value = PJsonValue()) { setJsonValue(value); }
    virtual void setJsonValue(const PJsonValue &value);
    virtual PJsonValue getJsonValue() const;
    virtual void setValue(const QString &jsonPath, const PJsonValue &value);
    virtual PJsonValue getValue(const QString &jsonPath) const;
};

}
}
#endif // PCONFIG_H
