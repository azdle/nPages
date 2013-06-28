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

    reloadTimer = new QTimer(this);
    connect(reloadTimer, SIGNAL(timeout()), this, SLOT(nextPage()));
    reloadTimer->start(paramMap["interval"].toInt());
}

PageWindow::~PageWindow(){
    reloadTimer->stop();
    pageView->close();
}

void PageWindow::nextPage(){
    if(pageIndex >= pages.size()){
        pageIndex = 0;
    }

    pageView->load(QUrl(pages[pageIndex]));

    pageIndex++;
}
