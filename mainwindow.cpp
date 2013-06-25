#include "mainwindow.h"
#include <vector>
#include <string>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    QString* url1 = new QString("https://status.exosite.com");
    QString* url2 = new QString("https://portals.exosite.com/views/2219771501/1385257981");
    std::vector<QString*> urls1(1);
    urls1[0] = url1;
    std::vector<QString*> urls2(1);
    urls2[0] = url2;
    views[0] = new PageWindow(parent, urls1, 0, 10000);
    views[1] = new PageWindow(parent, urls2, 1, 10000);
}

MainWindow::~MainWindow()
{
}
