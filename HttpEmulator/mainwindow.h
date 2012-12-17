#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QRegularExpression>

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
    void on_requestButton_clicked();
    void receiveReply(QNetworkReply* reply);

private:
    Ui::MainWindow *ui;
    QNetworkAccessManager manager;
};

#endif // MAINWINDOW_H
