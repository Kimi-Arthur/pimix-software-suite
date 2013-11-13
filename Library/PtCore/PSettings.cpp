#include "PSettings.h"

PSettings::PSettings()
{
}

PSettings::PSettings(const QString &fileName)
    : fileName(fileName)
{

}

QString PSettings::getFileName() const
{
    return fileName;
}

void PSettings::setFileName(const QString &value)
{
    fileName = value;
}
