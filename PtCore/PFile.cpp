#include "PFile.h"
#include <QFile>


PFile::PFile()
{
}

int PFile::mergeFiles(QStringList inputFileNameList, QString outputFileName)
{
    QFile outputFile(outputFileName);
    if (!outputFile.open(QIODevice::Append | QIODevice::Truncate)) {
        // TODO log
        return -1;
    }
    for (auto &fileName : inputFileNameList) {
        QFile inputFile(fileName);
        if (!inputFile.open(QIODevice::ReadOnly)) {
            // TODO log
            continue;
        }
        forever {
            qint64 count = outputFile.write(inputFile.read(BlockSize));
            outputFile.flush();
            if (count < BlockSize)
                break;
        }
        inputFile.close();
    }
    outputFile.close();
    return 0;
}

int PFile::splitFile(QString inputFileName, QList<QPair<QString, qint64> > outputFileInfos)
{
    qint64 totalSize = 0;
    for (QPair<QString, qint64> &info : outputFileInfos)
        totalSize += info.second;

    QFile inputFile(inputFileName);
    if (!inputFile.open(QIODevice::ReadOnly)) {
        // TODO log
        return -1;
    }
    if (inputFile.size() != totalSize) {
        // TODO log
        return -1;
    }

    for (QPair<QString, qint64> &info : outputFileInfos) {
        QFile outputFile(info.first);
        if (!outputFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            // TODO log
            return -1;
        }
        qint64 blockCount = (info.second - 1) / BlockSize + 1;
        for (qint64 i = 0; i < blockCount; ++i) {
            // Temp workaroud for MinGW bug
            qint64 sizeA = BlockSize, sizeB = info.second - i * BlockSize;
            outputFile.write(inputFile.read(qMin(sizeA, sizeB)));
            outputFile.flush();
        }
        outputFile.close();
    }
    inputFile.close();
    return outputFileInfos.size();
}

int PFile::splitFile(QString inputFileName, QString outputFileNamePrefix, qint64 outputFileSize)
{
    QFile inputFile(inputFileName);
    if (!inputFile.open(QIODevice::ReadOnly)) {
        // TODO log
        return -1;
    }
    qint64 inputFileSize = inputFile.size();
    QList<QPair<QString, qint64>> outputFileInfos;
    qint64 partCount = (inputFileSize - 1) / outputFileSize + 1;
    outputFileInfos.reserve(partCount);
    for (qint64 i = 0; i < partCount; ++i)
        outputFileInfos.append({outputFileNamePrefix + ".p" + QString("%1").arg(i, 2, 10, QChar('0')), qMin(outputFileSize, inputFileSize - i * outputFileSize)});
    return splitFile(inputFileName, outputFileInfos);
}
