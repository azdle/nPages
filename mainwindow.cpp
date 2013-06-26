#include "mainwindow.h"
#include <vector>
#include <string>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{

    // Manually Populating For Now. Will use file or QSettings in future.
    // Exosite Start
    cik = new QString("654a29f2f230a2810a28036f79ab2fef17c058c4");
    datasource_alias = new QString("sws");
    //Exosite End

    // For 1 Start
    QString* url1 = new QString("https://status.exosite.com");
    std::vector<QString*> urls1(1);
    urls1[0] = url1;
    PageWindow::parameters *thisSettings1 = new PageWindow::parameters;
    thisSettings1->urls = urls1;
    thisSettings1->display = 0;
    thisSettings1->interval = 10000;
    //pageSettings.push_back(thisSettings1);
    // For 1 End

    // For 2 Start
    QString* url2a = new QString("https://portals.exosite.com/views/2219771501/1385257981");
    QString* url2b = new QString("https://mkii.org");
    std::vector<QString*> urls2(2);
    urls2[0] = url2a;
    urls2[1] = url2b;
    PageWindow::parameters *thisSettings2 = new PageWindow::parameters;
    thisSettings2->urls = urls2;
    thisSettings2->display = 1;
    thisSettings2->interval = 10000;
    //pageSettings.push_back(thisSettings2);
    // For 2 End


    this->ReadFromExosite();

    /*std::vector<PageWindow::parameters*>::iterator iter;
    for (iter = pageSettings.begin(); iter != pageSettings.end(); iter++){
        views.push_back(new PageWindow(parent, (*iter)));
    }*/
}

MainWindow::~MainWindow()
{
}

void MainWindow::SaveSettings(){

}

void MainWindow::ReadFromExosite(){
    nam = new QNetworkAccessManager(this);
    QObject::connect(nam, SIGNAL(finished(QNetworkReply*)),
                     this, SLOT(finishedSlot(QNetworkReply*)));

    QString* base_url = new QString("https://m2.exosite.com/onep:v1/stack/alias?");

    QUrl url(base_url->append(datasource_alias));
    QNetworkRequest* request = new QNetworkRequest(url);
    request->setRawHeader(QByteArray("X-Exosite-CIK"), QByteArray("").append(*cik));
    request->setRawHeader(QByteArray("Accept"), QByteArray("application/x-www-form-urlencoded; charset=utf-8"));

    QNetworkReply* reply = nam->get(*request);
    // NOTE: Store QNetworkReply pointer (maybe into caller).
    // When this HTTP request is finished you will receive this same
    // QNetworkReply as response parameter.
    // By the QNetworkReply pointer you can identify request and response.
}

void MainWindow::finishedSlot(QNetworkReply* reply){
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
        std::cout << string.remove(datasource_alias->append("=")).toStdString() << std::endl;
        settings = qparse.parse(QByteArray("").append(string.remove(datasource_alias->append("="))), &ok);
        if(!ok){
            settings = QVariant();
        }else{

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
        std::cout << "HTTP Error" << statusCodeV.toInt() << std::endl;
    }

    // We receive ownership of the reply object
    // and therefore need to handle deletion.
    //delete reply;
}
