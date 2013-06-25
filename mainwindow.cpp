#include "mainwindow.h"
#include <vector>
#include <string>

#define NUMBER_OF_WINDOWS 2

#define PAGE_REFRESH_MS 10000

#define NUMBER_OF_PAGES 4

const QString url[] = {
        "https://portals.exosite.com/views/2219771501/1385257981",
        "https://portals.exosite.com/views/3819798770/3579834479",
        "https://portals.exosite.com/views/2562112089/4128069106",
        "https://portals.exosite.com/views/2141312486/3654335697"
      };

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    views[0] = new QWebView(parent);
    views[0]->load(QUrl("https://status.exosite.com"));
    views[0]->page()->mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
    views[0]->page()->mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);
    views[0]->setGeometry( QApplication::desktop()->screenGeometry(0));
    views[0]->showFullScreen();

    views[1] = new QWebView(parent);
    //views[1]->load(QUrl("https://portals.exosite.com/views/2219771501/1385257981"));
    this->nextPage();
    views[1]->page()->mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
    views[1]->page()->mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);
    views[1]->setGeometry( QApplication::desktop()->screenGeometry(1));
    views[1]->showFullScreen();

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(nextPage()));
    timer->start(PAGE_REFRESH_MS);
}

MainWindow::~MainWindow()
{
}

void MainWindow::nextPage(){
    static int index = 0;

    if(index >= NUMBER_OF_PAGES){
        index = 0;
    }
    views[1]->load(QUrl(url[index]));

    index++;
}
