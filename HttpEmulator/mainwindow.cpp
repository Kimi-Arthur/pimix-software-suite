#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "ptnetwork.h"
#include <QJsonDocument>
#include <QDebug>
#include <QSharedPointer>
#include <QMessageBox>
#include <QFile>
#include "PFileChooserWidget.h"

// Local functions
using namespace Pt::Network;

QString process(char input)
{
    if (input == '\t')
        return "\\t";
    if (input == '\n')
        return "\\n";
    if (input < 32)
        return QString::number(input, 16).rightJustified(2, '0', true);
    return QString(input);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(receiveReply(QNetworkReply*)));
    connect(&manager, SIGNAL(authenticationRequired(QNetworkReply*,QAuthenticator*)), this, SLOT(login(QNetworkReply*,QAuthenticator*)));
    manager.setCookieJar(&cookieJar);
    connect(ui->hexModeRadio, &QRadioButton::toggled, [=](bool state){if (state) ui->stackedWidget->setCurrentIndex(0);});
    connect(ui->rawModeRadio, &QRadioButton::toggled, [=](bool state){if (state) ui->stackedWidget->setCurrentIndex(0);});
    connect(ui->fileModeRadio, &QRadioButton::toggled, [=](bool state){if (state) ui->stackedWidget->setCurrentIndex(1);});
}

MainWindow::~MainWindow()
{
    //PSettings.save(this);
    delete ui;
}

void MainWindow::on_requestButton_clicked()
{
    QUrl url(ui->uriLine->text());
    QString hostname(url.host());
    QString method = ui->methodCombo->currentText();
    QString rawData = ui->dataEdit->toPlainText();
    QByteArray data;
    if (ui->rawModeRadio->isChecked())
        data = rawData.toUtf8();
    if (ui->hexModeRadio->isChecked()) {
        rawData = rawData.toUpper().remove(QRegularExpression("[^0-9A-F]"));
        for (int i = 0; i < rawData.size(); i += 2)
            data += rawData.mid(i, 2).toInt(0, 16);
    }
    QFile *f;
    if (ui->fileModeRadio->isChecked()) {
        qDebug() << ui->fileNameEdit->getPath();
        f = new QFile(ui->fileNameEdit->getPath());
        if (f->open(QIODevice::ReadOnly))
            qDebug() << "OK";
        qDebug() << f->errorString();
    }

    qDebug() << data.size();

    foreach (auto cookie, cookieJar.cookiesForUrl(hostname))
        cookieJar.deleteCookie(cookie);

    QString cookies = ui->cookieEdit->toPlainText();
    foreach (QString cookie, cookies.split(";")) {
        QStringList l = cookie.split("=");
        if (l.size() != 2)
            continue;
        QNetworkCookie o(l[0].toUtf8(), l[1].toUtf8());
        o.setDomain(hostname);
        cookieJar.insertCookie(o);
    }

    manager.setProxy(getProxy(ui->proxyLine->text()));
    qDebug() << manager.proxy();
    auto request = QNetworkRequest(url);
    //request.setHeader(QNetworkRequest::UserAgentHeader, "test");
    //request.setRawHeader("Authorization", "Basic a2ltaS5yaWJlcnlAZ21haWwuY29tOmU2R0VveVFKdmhvbkswS3YvUGpDNCtNZUdUczlOTFFQZWdYWWJCdWJmN1U=");

    //request.setRawHeader("X-MS-DataAcquisition-Tenant", "f7ca8e11-c205-4c06-9c63-ed002f21c10c");
    if (method == "GET")
        manager.get(request);
    if (method == "POST") {
        manager.post(request, data);
    }
    if (method == "PUT") {
        //request.setRawHeader("X-CEVALTOKEN", "5Dm6jD4tz1XYwA2UyDF4uQ773Y2B9zzNAAABP8YyoPgI9Mk_47TAjMpkVc70F-fFkyromg");
        manager.put(request, f);
    }
    if (method == "DELETE")
        manager.deleteResource(request);
}

void MainWindow::receiveReply(QNetworkReply *reply)
{
    QMessageBox::information(this, "Success", "Reply received!");
    QByteArray content = reply->readAll();
    qDebug() << content;
    ui->rawOutput->clear();
    ui->hexOutput->clear();
    QString hexLine, rawLine;
    QByteArray hexContent = content.toHex();
    for (int i = 0; i < content.size(); i++) {
        if (!(i & 0xF)) {
            if (i) {
                ui->hexOutput->append(hexLine);
                ui->rawOutput->append(rawLine);
            }
            rawLine = hexLine = QStringLiteral("0x%1").arg(QString::number(i, 16).rightJustified(4, '0', true));
        }
        rawLine.append(QStringLiteral("%1").arg(process(content.at(i)), 3, QChar(' ')));
        hexLine.append(QStringLiteral(" %1%2").arg(hexContent.at(i * 2)).arg(hexContent.at(i * 2 + 1)));
    }
    ui->hexOutput->append(hexLine);
    ui->rawOutput->append(rawLine);
    ui->htmlOutput->setHtml(content);
    ui->hexStreamOutput->setText(hexContent.toUpper());;
    ui->rawStreamOutput->setText(content);
    ui->jsonOutput->setText(QJsonDocument::fromJson(content).toJson());
    reply->deleteLater();
}

void MainWindow::login(QNetworkReply *reply, QAuthenticator *auther)
{
    qDebug() << "Authing";
    auther->setUser(ui->usernameLine->text());
    auther->setPassword(ui->passwordLine->text());
}
