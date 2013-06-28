#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QMainWindow>
#include <QtWebKit>
#include <QDesktopWidget>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QProgressBar>
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

    // Exosite Settings
    QString* cik;
    QString* datasource_alias;

    QJson::Parser qparse;
    QJson::Serializer qserial;

    QVariant settings;      // Page Settings from Exosite, Fetched Fresh Every Run (and Every Hour After That)
    QSettings* exoSettings;  // Client Settings for Identifying to Exosite, Saved Locally

    QTimer* windowTimeout;

    // UI Elements
    QWidget* masterWidget;
    QVBoxLayout* masterLayout;
    QHBoxLayout* saveButtonBox;
    QFormLayout* formLayout;
    QGroupBox* formGroup;

    QPushButton* saveButton;
    QPushButton* cancelButton;

    QLineEdit* cikEdit;
    QLineEdit* aliasEdit;

    QLabel* cikLabel;
    QLabel* aliasLabel;

    QProgressBar* timeoutProgress;

private slots:
    void NetworkReply(QNetworkReply* reply);
    void ReadFromExosite();
    void StartPages();
    void SaveSettings();
    void WindowTimerTick();
    bool CheckSettingsFields();
};

#endif // MAINWINDOW_H
