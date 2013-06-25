#include "pagewindow.h"

PageWindow::PageWindow(QWidget *parent, PageWindow::parameters* parameters) :
    QDialog(parent)
{
    pages = parameters->urls;
    pageView = new QWebView(parent);
    pageView->load(QUrl(*parameters->urls[0]));
    pageView->page()->mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
    pageView->page()->mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);
    pageView->setGeometry( QApplication::desktop()->screenGeometry(parameters->display));
    pageView->showFullScreen();

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(nextPage()));
    timer->start(parameters->interval);
}


void PageWindow::nextPage(){
    static unsigned int index = 0;

    if(index >= pages.size()){
        index = 0;
    }
    pageView->load(QUrl(*pages[index]));

    index++;
}
