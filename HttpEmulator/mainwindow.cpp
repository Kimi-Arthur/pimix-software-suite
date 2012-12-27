#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

// Local functions
QNetworkProxy getProxy(const QString &proxyString);

QString process(char input)
{
    if (input == '\t')
        return "\\t";
    if (input == '\n')
        return "\\n";
    if (input < 32)
        return QString::number(input, 16).rightJustified(2, '0', true);
    return input;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    connect(&manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(receiveReply(QNetworkReply*)));
    manager.setCookieJar(&cookieJar);
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_requestButton_clicked()
{
    QUrl url(ui->uriLine->text());
    QString hostname(url.host());
    QString method = ui->methodCombo->currentText();
    QString rawData = ui->dataEdit->toPlainText();
    QByteArray data;
    if (ui->rawModeButton->isChecked())
        data = rawData.toUtf8();
    if (ui->hexModeRadio->isChecked()) {
        rawData = rawData.toUpper().remove(QRegularExpression("[^0-9A-F]"));
        for (int i = 0; i < rawData.size(); i += 2)
            data += rawData.mid(i, 2).toInt(0, 16);
    }
    if (ui->fileModeButton->isChecked()) {
        data = "";
    }

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

    if (method == "GET")
        manager.get(QNetworkRequest(url));
    if (method == "POST") {
        manager.post(QNetworkRequest(url), data);
    }
    if (method == "PUT")
        manager.put(QNetworkRequest(url), data);
    if (method == "DELETE")
        manager.deleteResource(QNetworkRequest(url));
}

void MainWindow::receiveReply(QNetworkReply *reply)
{
    QByteArray content = reply->readAll();
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
    if (content.size() & 0xF) {
        ui->hexOutput->append(hexLine);
        ui->rawOutput->append(rawLine);
    }
    ui->htmlOutput->setHtml(content);
    ui->hexStreamOutput->setText(hexContent.toUpper());;
    ui->rawStreamOutput->setText(content);
    reply->deleteLater();
}

QNetworkProxy getProxy(const QString &proxyString)
{
    auto result = QRegularExpression("^((?<username>.*):(?<password>.*)@)?(?<hostname>[\\.a-zA-Z0-9]+):(?<port>\\d+)$").match(proxyString);
    if (result.hasMatch())
        return QNetworkProxy(QNetworkProxy::DefaultProxy, result.captured("hostname"), result.captured("port").toInt(), result.captured("username"), result.captured("password"));
    else return QNetworkProxy();
}
