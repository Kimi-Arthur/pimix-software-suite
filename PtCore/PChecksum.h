#ifndef PCHECKSUM_H
#define PCHECKSUM_H

#include "PtCore_global.h"

class PChecksumMethod;

template <class Key, class T>
class QMap;

class PTCORESHARED_EXPORT PChecksum
{
public:
    enum class Algorithm {
        Crc16,
        Crc32,
        Crc64
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

#endif // PCHECKSUM_H
