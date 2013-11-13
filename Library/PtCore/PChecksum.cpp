#include "PChecksum.h"
#include <QBitArray>
#include <QMap>
#include <zlib.h>

using namespace Pt::Core;

class PChecksumMethod
{
public:
    PChecksumMethod() {}
    virtual void reset() = 0;
    virtual void addData(const QByteArray &data) = 0;
    virtual QByteArray result() const = 0;
};

class PChecksumMethodCrc32 : public PChecksumMethod
{
    quint32 state;

public:
    PChecksumMethodCrc32()
        : PChecksumMethod()
    {
        reset();
    }

    virtual void reset()
    {
        state = 0;
    }

    virtual void addData(const QByteArray &data)
    {
        state = crc32(state, (const unsigned char *)data.constData(), data.length());
    }

    virtual QByteArray result() const
    {
        QByteArray result(4, 0);
        for (int i = 0; i < 4; ++i)
            result[4 - i - 1] = state >> (i << 3) & 0xFF;
        return result;
    }

};

QMap<PChecksum::Algorithm, std::function<PChecksumMethod *()>> PChecksum::checksumMethods =
{
    {PChecksum::Algorithm::Crc32, []() { return new PChecksumMethodCrc32(); }}
};



PChecksum::PChecksum(Algorithm method)
{
    checksumMethod = PChecksum::checksumMethods.value(method)();
    reset();
}

PChecksum::~PChecksum()
{
    delete checksumMethod;
}

void PChecksum::reset()
{
    checksumMethod->reset();
}

void PChecksum::addData(const QByteArray &data)
{
    checksumMethod->addData(data);
}

QByteArray PChecksum::result() const
{
    return checksumMethod->result();
}
