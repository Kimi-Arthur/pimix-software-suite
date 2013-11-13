#include "PConsole.h"

PConsole::PConsole()
{
}

QStringList PConsole::extractParameters(int argc, char *argv[])
{
    QStringList result;
    for (int index = 0; index < argc; ++index)
        result.append(argv[index]);
    return result;
}
