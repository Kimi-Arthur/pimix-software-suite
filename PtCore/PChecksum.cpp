#include "PChecksum.h"
#include <QMap>

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
public:
    PChecksumMethodCrc32()
        : PChecksumMethod()
    {

    }

    virtual void reset()
    {

    }

    virtual void addData(const QByteArray &data)
    {

    }

    virtual QByteArray result() const
    {
        return QByteArray();
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
