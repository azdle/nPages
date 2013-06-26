#include "pagewindow.h"
#include <iostream>

PageWindow::PageWindow(QWidget *parent, PageWindow::parameters* parameters) :
    QDialog(parent)
{
    pageIndex = 0;

    pages = parameters->urls;
    pageView = new QWebView(parent);
    pageView->page()->mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
    pageView->page()->mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);
    pageView->setGeometry( QApplication::desktop()->screenGeometry(parameters->display));
    pageView->showFullScreen();
    this->nextPage();

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(nextPage()));
    timer->start(parameters->interval);
}


void PageWindow::nextPage(){

    std::cout << "Size: " << pages.size() << "Current Index: " << pageIndex << std::endl;

    if(pageIndex >= pages.size()){
        pageIndex = 0;
        std::cout << "Resetting Index" << std::endl;
    }
    if(pages[pageIndex]){
        std::cout << "Url: " << pages[pageIndex] << std::endl << std::endl;
        pageView->load(QUrl(*pages[pageIndex]));
    }

    pageIndex++;
}
