#include <QCoreApplication>
#include <QJsonDocument>
#include <QFile>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QFile f("C:/Users/jingbian/Desktop/catalog.test");
    f.open(QIODevice::ReadOnly);
    QJsonDocument doc = QJsonDocument::fromJson(f.readAll());
    QFile ff("C:/Users/jingbian/Desktop/catalog.test.json");
    ff.open(QIODevice::WriteOnly);
    QTextStream stream(&ff);
    stream << doc.toJson();
}
