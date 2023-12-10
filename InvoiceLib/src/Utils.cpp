#include "Utils.h"

#include <QFile>
#include <QTextStream>

QString Utils::readFileContent(const QString &filename)
{
    QFile f(filename);
    if (!f.open(QFile::ReadOnly | QFile::Text))
        return QString();
    QTextStream in(&f);
    return in.readAll();
}


