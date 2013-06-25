#include "pagewindow.h"

PageWindow::PageWindow(QWidget *parent, std::vector<QString*> urls, int display, int interval) :
    QDialog(parent)
{
    pages = urls;
    pageView = new QWebView(parent);
    pageView->load(QUrl(*urls[0]));
    pageView->page()->mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
    pageView->page()->mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);
    pageView->setGeometry( QApplication::desktop()->screenGeometry(display));
    pageView->showFullScreen();

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(nextPage()));
    timer->start(interval);
}


void PageWindow::nextPage(){
    static int index = 0;

    if(index >= pages.size()){
        index = 0;
    }
    pageView->load(QUrl(*pages[index]));

    index++;
}
