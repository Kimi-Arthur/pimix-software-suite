#include "PString.h"

Pt::Core::PString::PString()
{
}

QString Pt::Core::PString::format(QString pattern, std::map<QString, QString> parameters)
{
    for (auto parameter : parameters)
        pattern.replace(QString("{%1}").arg(parameter.first), parameter.second);
    return pattern;
}
