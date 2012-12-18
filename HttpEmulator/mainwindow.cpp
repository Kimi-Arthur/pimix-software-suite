#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    connect(&manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(receiveReply(QNetworkReply*)));
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_requestButton_clicked()
{
    QString uri = ui->uriLine->text();
    QString method = ui->methodCombo->currentText();
    QString raw_data = ui->dataEdit->toPlainText();
    QByteArray data;
    if (ui->dataModeCombo->currentText() == "Hex Mode") {
        raw_data = raw_data.toUpper().remove(QRegularExpression("[^0-9A-F]"));
        for (int i = 0; i < raw_data.size(); i += 2)
            data += raw_data.mid(i, 2).toInt(0, 16);
    } else {
        data = raw_data.toUtf8();
    }

    QString cookies = ui->cookieEdit->toPlainText();
    if (method == "GET")
        manager.get(QNetworkRequest(uri));
    if (method == "POST") {
        manager.post(QNetworkRequest(uri), data);
    }
    if (method == "PUT")
        manager.put(QNetworkRequest(uri), data);
    if (method == "DELETE")
        manager.deleteResource(QNetworkRequest(uri));
}

void MainWindow::receiveReply(QNetworkReply *reply)
{
    QByteArray content = reply->readAll();
    ui->rawOutput->clear();
    ui->hexOutput->clear();
    QString hexLine, rawLine;
    for (int i = 0; i < content.size(); i++) {
        if (!(i & 0xF)) {
            if (i) {
                ui->hexOutput->append(hexLine);
                ui->rawOutput->append(rawLine);
            }
            rawLine = hexLine = QStringLiteral("0x%1").arg(QString::number(i, 16).rightJustified(4, '0', true));
        }
        QString number = QStringLiteral(" %1").arg(QString::number(content.at(i), 16).rightJustified(2, '0', true));
        if (content.at(i) > 0 && content.at(i) < 128)
            rawLine.append(QStringLiteral("%1").arg(content.at(i), 3, QChar(' ')));
        else
            rawLine.append(number);
        hexLine.append(number);
    }
    if (content.size() & 0xF) {
        ui->hexOutput->append(hexLine);
        ui->rawOutput->append(rawLine);
    }
    reply->deleteLater();
}
