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
    if(pageIndex >= pages.size()){
        pageIndex = 0;
    }

    if(pages[pageIndex]){ // Can Sometimes Be Zero, Without Check -> Crash
        pageView->load(QUrl(*pages[pageIndex]));
    }else{
        std::cout << "Error: Page Pointer was Zero" << std::endl;
    }

    pageIndex++;
}
