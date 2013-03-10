#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QFormLayout * layout = new QFormLayout();
    ui->outputGroupBox->setLayout(layout);
    QString alg[] = {"MD4", "MD5", "SHA-1", "SHA-224", "SHA-256", "SHA-384", "SHA-512"};
    int algcount = 7;
    for (int i = 0; i < algcount; i++) {
        QString name = alg[i];
        algorithms.append(name);
        outputLines.append(new QLineEdit());
        outputLines.back()->setReadOnly(false);
        layout->addRow(name, outputLines.back());
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_rawTextModeRadio_toggled(bool checked)
{
    if (checked)
        ui->sourceStackedWidget->setCurrentIndex(0);
}

void MainWindow::on_hexTextModeRadio_toggled(bool checked)
{
    if (checked)
        ui->sourceStackedWidget->setCurrentIndex(1);
}

void MainWindow::on_fileModeRadio_toggled(bool checked)
{
    if (checked)
        ui->sourceStackedWidget->setCurrentIndex(2);
}

void MainWindow::on_hashButton_clicked()
{
    if (ui->rawTextModeRadio->isChecked()) {
        for (int algorithm = 0; algorithm < algorithms.size(); algorithm++) {
            QCryptographicHash::Algorithm alg = QCryptographicHash::Algorithm(algorithm);
            QCryptographicHash hasher(alg);
            hasher.addData(ui->rawTextEdit->document()->toPlainText().toUtf8());
            outputLines[algorithm]->setText(hasher.result().toHex().toUpper());
        }
    }
    if (ui->hexTextModeRadio->isChecked()) {
        QString text = ui->hexTextEdit->document()->toPlainText().toUpper().remove(QRegularExpression("[^0-9A-F]"));
        QByteArray hexData;
        for (int i = 0; i < text.size(); i += 2)
            hexData += text.mid(i, 2).toInt(0, 16);
        for (int algorithm = 0; algorithm < algorithms.size(); algorithm++) {
            QCryptographicHash::Algorithm alg = QCryptographicHash::Algorithm(algorithm);
            QCryptographicHash hasher(alg);
            hasher.addData(hexData);
            outputLines[algorithm]->setText(hasher.result().toHex().toUpper());
        }
    }
    if (ui->fileModeRadio->isChecked()) {
        QFile file(ui->fileWidget->getPath());
        file.open(QIODevice::ReadOnly);
        for (int algorithm = 0; algorithm < algorithms.size(); algorithm++) {
            file.reset();
            QCryptographicHash::Algorithm alg = QCryptographicHash::Algorithm(algorithm);
            QCryptographicHash hasher(alg);
            outputLines[algorithm]->setText(hasher.result().toHex().toUpper());
        }
    }
}

