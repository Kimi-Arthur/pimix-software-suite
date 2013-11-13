#ifndef PCHECKSUM_H
#define PCHECKSUM_H

#include "PtCore_global.h"


template <class Key, class T>
class QMap;

class PChecksumMethod;

namespace Pt {
namespace Core {

class PTCORESHARED_EXPORT PChecksum
{
public:
    enum class Algorithm {
        Crc32
    };

    PChecksum(Algorithm method);
    ~PChecksum();
    void reset();
    void addData(const QByteArray &data);
    QByteArray result() const;

private:
    static QMap<Algorithm, std::function<PChecksumMethod *()>> checksumMethods;
    PChecksumMethod *checksumMethod;
    Algorithm methodType;
};

}
}

#endif // PCHECKSUM_H
