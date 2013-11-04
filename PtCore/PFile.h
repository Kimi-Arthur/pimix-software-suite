#ifndef PFILE_H
#define PFILE_H

#include "PtCore_global.h"

class PTCORESHARED_EXPORT PFile
{
public:
    static const qint64 ByteSize = 1;
    static const qint64 KilobyteSize = ByteSize << 10;
    static const qint64 MegabyteSize = ByteSize << 20;
    static const qint64 GigabyteSize = ByteSize << 30;
    static const qint64 TerabyteSize = ByteSize << 40;
    static const qint64 PetabyteSize = ByteSize << 50;
    static const qint64 ExabyteSize = ByteSize << 60;
    static const qint64 BlockSize = 4 * MegabyteSize;
    PFile();
    static int mergeFiles(QStringList inputFileNameList, QString outputFileName);
    static int splitFile(QString inputFileName, QList<QPair<QString, qint64> > outputFileInfos);
    static int splitFile(QString inputFileName, QString outputFileNamePrefix, qint64 outputFileSize = 2 * GigabyteSize);
};

#endif // PFILE_H
