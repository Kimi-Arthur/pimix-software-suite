#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QRegularExpression>
#include <QNetworkCookieJar>
#include <QNetworkCookie>
#include <QNetworkProxy>
#include <QAuthenticator>

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
    void login(QNetworkReply* reply, QAuthenticator* auther);

private:
    Ui::MainWindow *ui;
    QNetworkAccessManager manager;
    QNetworkCookieJar cookieJar;
};

#endif // MAINWINDOW_H
