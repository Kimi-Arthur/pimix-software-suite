#include "PString.h"

#include <QDebug>

Pt::Core::PString::PString()
{
}

QString Pt::Core::PString::format(QString pattern, PStringMap parameters)
{
    for (auto parameter : parameters)
        pattern.replace(QString("{%1}").arg(parameter.first), parameter.second);
    return pattern;
}
