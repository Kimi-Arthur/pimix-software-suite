#include <QCoreApplication>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QPluginLoader>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QTextStream input(stdin), output(stdout);
    while (1) {
        output << ">";
        output.flush();
        QString rawCommand;
        rawCommand = input.readLine();
        QJsonObject command = QJsonDocument::fromJson(rawCommand.toUtf8()).object();
        qDebug() << command;
        qDebug() << "C:/Users/jingbian/Projects/Capricorn/bin/" + command["uri"].toString() + ".dll";
        QPluginLoader loader("C:/Users/jingbian/Projects/Capricorn/bin/" + command["uri"].toString() + ".dll");
        qDebug() << loader.load();
    }
}
