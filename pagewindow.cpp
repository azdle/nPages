#include "pagewindow.h"
#include <iostream>

PageWindow::PageWindow(QWidget *parent, QVariant parameters) :
    QDialog(parent)
{
    pageIndex = 0;
    QVariantMap paramMap = parameters.toMap();

    pages = paramMap["urls"].toStringList();
    pageView = new QWebView(parent);
    pageView->page()->mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
    pageView->page()->mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);
    pageView->setGeometry( QApplication::desktop()->screenGeometry(paramMap["display"].toInt()));
    pageView->showFullScreen();
    this->nextPage();

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(nextPage()));
    timer->start(paramMap["interval"].toInt());
}


void PageWindow::nextPage(){
    if(pageIndex >= pages.size()){
        pageIndex = 0;
    }

    pageView->load(QUrl(pages[pageIndex]));

    pageIndex++;
}
