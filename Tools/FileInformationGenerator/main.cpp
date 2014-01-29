#include <QCoreApplication>
#include "PConsole.h"

#include <QDebug>
#include <QDir>
#include <QString>
#include <QFileInfoList>
#include <QFile>
#include <QFileInfo>
#include <QDate>
#include <QJsonObject>
#include <QCryptographicHash>
#include "PtCore.h"
#include "PFile.h"

QJsonObject getHashes(QString filePath)
{
    QFile f(filePath);
    qDebug() << f.open(QIODevice::ReadOnly);
    QByteArray content;
    while (content = f.read(PFile::BlockSize)) {
        QCryptographicHash::hash(content, );
    }
}

QByteArray getFileInformation(QFileInfo fileInfo)
{
    QJsonObject information;
    information["LastModified"] = QJsonValue(Pt::dateTimeToString(fileInfo.lastModified(), Qt::ISODate));
    information["Size"] = QString::number(fileInfo.size());
    information["Blocks"] = getHashes(fileInfo.absoluteFilePath());
    //information[QString("LastModified")] = QJsonValue(Pt::dateTimeToString(fileInfo.lastModified(), Qt::ISODate));
    qDebug() << information;
}

void generateFileInformation(QDir fromBaseDir, QDir toBaseDir, QString relativeFilePath)
{
    //qDebug() << fromBaseDir.absolutePath() << toBaseDir.absolutePath() << relativeFilePath;
    toBaseDir.mkpath(relativeFilePath);
    QFileInfo(fromBaseDir, relativeFilePath);
    getFileInformation(QFileInfo(fromBaseDir, relativeFilePath));
//    QFile infoFile(QFileInfo(toBaseDir, relativeFilePath + "/FileInfo.file").absoluteFilePath());
//    if (!infoFile.open(QIODevice::WriteOnly))
//        return;
//    infoFile.write(getFileInformation(QFileInfo(fromBaseDir, relativeFilePath)));
//    infoFile.close();
}

void visit(QDir fromBaesDir, QDir currentDir, QDir toBaseDir)
{
    foreach (QFileInfo entry, currentDir.entryInfoList(QDir::Files)) {
        generateFileInformation(fromBaesDir, toBaseDir, fromBaesDir.relativeFilePath(entry.absoluteFilePath()));
    }
    foreach (QFileInfo entry, currentDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        visit(fromBaesDir, entry.absoluteFilePath(), toBaseDir);
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QStringList parameters = PConsole::extractParameters(argc, argv);
    //P_ASSERT()
    qDebug() << parameters;
    QDir server(parameters[1]);
    visit(parameters[1], parameters[1], parameters[2]);
}
