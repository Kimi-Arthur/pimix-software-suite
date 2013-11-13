#ifndef PFILE_H
#define PFILE_H

#include "PtCore_global.h"

class PTCORESHARED_EXPORT PFile
{
public:
    static const qint64 ByteSize;
    static const qint64 KilobyteSize;
    static const qint64 MegabyteSize;
    static const qint64 GigabyteSize;
    static const qint64 TerabyteSize;
    static const qint64 PetabyteSize;
    static const qint64 ExabyteSize;
    static const qint64 BlockSize;
    PFile();
    static int mergeFiles(QStringList inputFileNameList, QString outputFileName);
    static int splitFile(QString inputFileName, QList<QPair<QString, qint64> > outputFileInfos);
    static int splitFile(QString inputFileName, QString outputFileNamePrefix, qint64 outputFileSize = 2 * GigabyteSize);
};

#endif // PFILE_H
