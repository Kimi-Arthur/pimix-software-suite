#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCryptographicHash>
#include <QRegularExpression>
#include <QLineEdit>
#include <QFormLayout>
#include "filechooserwidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_rawTextModeRadio_toggled(bool checked);

    void on_hexTextModeRadio_toggled(bool checked);

    void on_fileModeRadio_toggled(bool checked);

    void on_hashButton_clicked();

private:
    Ui::MainWindow *ui;
    QStringList algorithms;
    // Better implementation
    //    QStringList algorithms = {"MD4", "MD5", "SHA-1", "SHA-224", "SHA-256", "SHA-384", "SHA-512"};
    QList<QLineEdit *> outputLines;
};

#endif // MAINWINDOW_H
