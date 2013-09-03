#ifndef PSETTINGS_H
#define PSETTINGS_H

#include <QString>
#include <QMap>
#include <QSharedPointer>
#include "ptcore_global.h"

class PSettings
{
    QString fileName;
public:
    typedef QMap<QString, QPair<QSharedPointer<QObject>, QString> > PSettingsProfile;
    PSettings();
    PSettings(const QString &fileName);

    QString getFileName() const;
    void setFileName(const QString &value);
    //void save(QString name="", PSettingsProfile profile);
    //void load(QString name="", PSettingsProfile profile);
};

#endif // PSETTINGS_H
