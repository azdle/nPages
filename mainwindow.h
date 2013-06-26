#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QMainWindow>
#include <QtWebKit>
#include <QDesktopWidget>
#include <QSettings>
#include <QtNetwork/QNetworkAccessManager>
#include <QUrl>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <qjson/parser.h>
#include <qjson/serializer.h>
#include <vector>
#include <pagewindow.h>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    std::vector<PageWindow::parameters*> pageSettings;
    std::vector<PageWindow*> views;
    QNetworkAccessManager* nam;

    void SaveSettings();
    void ReadFromExosite();

    // Exosite Settings
    QString* cik;
    QString* datasource_alias;

    QJson::Parser qparse;
    QJson::Serializer qserial;

    QVariant settings;

private slots:
    void finishedSlot(QNetworkReply* reply);
};

#endif // MAINWINDOW_H
