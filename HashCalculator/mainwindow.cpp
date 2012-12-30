#include "mainwindow.h"
#include "ui_mainwindow.h"

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
