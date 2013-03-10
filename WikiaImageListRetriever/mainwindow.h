#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>

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
    void on_retriveButton_clicked();
    void receiveImageList(QNetworkReply *reply);

private:
    Ui::MainWindow *ui;
    QJsonObject result;
    QString urlPrefix;
    QNetworkAccessManager *manager;
};

#endif // MAINWINDOW_H
