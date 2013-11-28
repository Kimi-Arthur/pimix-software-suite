#ifndef PDATAMODEL_H
#define PDATAMODEL_H


#include "PtCore_global.h"

#include "PJsonValue.h"

namespace Pt {
namespace Core {

#define DataMember(member_name)

class PTCORESHARED_EXPORT PDataModel
{
protected:
    PJsonValue jsonValue;

public:
    PDataModel(const PJsonValue &value) { setJsonValue(value); }
    virtual void setJsonValue(const PJsonValue &value) { jsonValue = value; }
    virtual PJsonValue getJsonValue() const { return jsonValue; }
    virtual void setValue(const QString &jsonPath, const PJsonValue &value) { }
    virtual PJsonValue getValue(const QString &jsonPath) const { }
};

}
}

#endif // PDATAMODEL_H
