#include "mainwindow.h"
#include <vector>
#include <string>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    exoSettings = new QSettings("Exosite", "nPages");
    cik = new QString(exoSettings->value("cik").toString());
    datasource_alias = new QString(exoSettings->value("alias").toString());

    windowTimeout = new QTimer(this);
    connect(windowTimeout, SIGNAL(timeout()), this, SLOT(WindowTimerTick()));
    windowTimeout->start(100);

    // Allocate All Widgets
    masterWidget = new QWidget();
    masterLayout = new QVBoxLayout(masterWidget);

    saveButtonBox = new QHBoxLayout();
    formLayout = new QFormLayout();
    formGroup = new QGroupBox("Exosite Settings");

    saveButton = new QPushButton(tr("Save Settings"));
    cancelButton = new QPushButton(tr("Use Existing Settings"));

    cikEdit = new QLineEdit();
    aliasEdit = new QLineEdit();
    cikLabel = new QLabel(tr("Client Identity Key"));
    aliasLabel = new QLabel(tr("Datasource Alias"));

    timeoutProgress = new QProgressBar();

    // Setup Widget Actions
    connect(saveButton, SIGNAL(clicked()), this, SLOT(SaveSettings()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(hide()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(StartPages()));

    // Setup Widget Values
    cikEdit->setText(*cik);
    aliasEdit->setText(*datasource_alias);

    timeoutProgress->setTextVisible(false);
    timeoutProgress->setMinimum(0);
    timeoutProgress->setMaximum(100);
    timeoutProgress->setInvertedAppearance(true);
    timeoutProgress->setValue(100);

    // Setup Widget Hierarchy
    setCentralWidget(masterWidget);

    saveButtonBox->addWidget(saveButton);
    saveButtonBox->addWidget(cancelButton);

    formLayout->addWidget(cikLabel);
    formLayout->addWidget(cikEdit);
    formLayout->addWidget(aliasLabel);
    formLayout->addWidget(aliasEdit);

    formGroup->setLayout(formLayout);
    masterLayout->addWidget(timeoutProgress);
    masterLayout->addWidget(formGroup);
    masterLayout->addLayout(saveButtonBox);
}

MainWindow::~MainWindow(){
}

void MainWindow::WindowTimerTick(){
    static int countdown = 100; // 30 Sec

    if(CheckSettingsFields()){
        countdown--;
        if(countdown <= 0){
            windowTimeout->stop();
            this->hide();
            this->StartPages();
        }
    }else{
        countdown = 100;
    }

    timeoutProgress->setValue(countdown);
}

bool MainWindow::CheckSettingsFields(){
    if(cikEdit->text().length() != 40){
        return false;
    }else if(aliasEdit->text().length() == 0){
        return false;
    }else{
        return true;
    }
}

void MainWindow::StartPages(){
    this->ReadFromExosite();

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(ReadFromExosite()));
    timer->start(3600000);
}

void MainWindow::ReadFromExosite(){
    nam = new QNetworkAccessManager(this);
    QObject::connect(nam, SIGNAL(finished(QNetworkReply*)),
                     this, SLOT(NetworkReply(QNetworkReply*)));

    std::cout << "Requesting with CIK '" << *cik->toAscii() << "' and alias'" << *datasource_alias->toAscii() << "'." << std::endl;

    QString* base_url = new QString("http://m2.exosite.com/onep:v1/stack/alias?");

    QUrl url(base_url->append(datasource_alias));
    QNetworkRequest* request = new QNetworkRequest(url);
    request->setRawHeader(QByteArray("X-Exosite-CIK"), QByteArray("").append(*cik));
    request->setRawHeader(QByteArray("Accept"), QByteArray("application/x-www-form-urlencoded; charset=utf-8"));

    nam->get(*request);

}

void MainWindow::NetworkReply(QNetworkReply* reply){
    // Reading attributes of the reply
    // e.g. the HTTP status code
    QVariant statusCodeV =
    reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    // Or the target URL if it was a redirect:
    QVariant redirectionTargetUrl =
    reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    // see CS001432 on how to handle this

    bool ok;

    // no error received?
    if (reply->error() == QNetworkReply::NoError)
    {
        // Example 2: Reading bytes form the reply
        QByteArray bytes = reply->readAll();  // bytes
        QString string = QUrl::fromPercentEncoding(bytes); // string
        string.remove(datasource_alias->append("")).remove("=");
        std::cout << "Received Settings with Status " << statusCodeV.toInt() << ": " << string.toStdString() << std::endl;
        settings = qparse.parse(QByteArray("").append(string), &ok);
        if(!ok){
            settings = QVariant();
            std::cout << "Parse Error!" << std::endl;
        }else{
            foreach(PageWindow* thisView, views){
                delete thisView;
            }
            views.clear();

            foreach(QVariant thisPageSettings, settings.toList()){
                std::cout << "Count" << std::endl;
                views.push_back(new PageWindow(0, thisPageSettings));
            }
        }
    }
    // Some http error received
    else
    {
        // handle errors here
        std::cout << "HTTP Error: " << statusCodeV.toInt() << std::endl;
    }

    // We receive ownership of the reply object
    // and therefore need to handle deletion.
    //delete reply;
}

void MainWindow::SaveSettings(){
    if(CheckSettingsFields()){
        exoSettings->setValue("cik", cikEdit->text());
        exoSettings->setValue("alias", aliasEdit->text());

        cik = new QString(exoSettings->value("cik").toString());
        datasource_alias = new QString(exoSettings->value("alias").toString());

        this->hide();
        this->StartPages();
    }
}
