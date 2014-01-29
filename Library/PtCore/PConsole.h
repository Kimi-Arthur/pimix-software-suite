#ifndef PCONSOLE_H
#define PCONSOLE_H

#include "PtCore_global.h"
#include <QStringList>

class PTCORESHARED_EXPORT PConsole
{
public:
    //typedef void (*ConsoleExecutionMethod)(QStringList);
    PConsole();
    static QStringList extractParameters(int argc, char *argv[]);
};

#endif // PCONSOLE_H
