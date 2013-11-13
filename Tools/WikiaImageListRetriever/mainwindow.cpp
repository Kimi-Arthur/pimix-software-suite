#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_retriveButton_clicked()
{
    manager = new QNetworkAccessManager;
    QString siteUrl = ui->siteUrlLine->text(); // Need normalize
    urlPrefix = siteUrl + "/api.php?action=query&format=json&generator=allimages&gailimit=500"
            + "&prop=revisions|imageinfo&rvprop=ids&iiprop=url";
    qDebug() << urlPrefix;
    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(receiveImageList(QNetworkReply*)));
    manager->get(QNetworkRequest(QUrl(urlPrefix)));
}

void MainWindow::receiveImageList(QNetworkReply *reply)
{
    //QString s(QString::fromUtf8(reply->readAll()));
//    auto a = reply->readAll();
//    for (int i = 0; i < a.size(); i++)
//        qDebug() << a.at(i);
//    qDebug() << QString::fromUtf8(reply->readAll());
    QJsonObject data(QJsonDocument::fromJson(reply->readAll()).object());
    QJsonObject imageList(data["query"].toObject()["pages"].toObject());
    foreach (auto image, imageList) {
        auto im = image.toObject();
        QString title = im["title"].toString().mid(5);
        QString url = im["imageinfo"].toArray()[0].toObject()["url"].toString();
        QString revid = QString::number(im["revisions"].toArray()[0].toObject()["revid"].toDouble());
        QJsonObject imageinfo;
        imageinfo["title"] = title;
        imageinfo["revision"] = QString(revid);
        imageinfo["url"] = url;
        result[title] = QJsonValue(imageinfo);
    }
    qDebug() << result.size();

    if (!data.contains("query-continue")) {
        qDebug() << "no more";
        QFile output("ImageList.txt");
        if (output.open(QIODevice::WriteOnly))
                output.write(QJsonDocument(result).toJson());
        return;
    }
    QUrl url(urlPrefix + "&gaifrom="
             + data["query-continue"].toObject()["allimages"].toObject()["gaifrom"].toString());
    manager->get(QNetworkRequest(QUrl(url)));
    qDebug() << url;

    reply->deleteLater();
}
