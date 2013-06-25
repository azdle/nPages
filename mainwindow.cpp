#include "mainwindow.h"
#include <vector>
#include <string>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    // Manually Populating For Now. Will use file or QSettings in future.
    // For 1 Start
    QString* url1 = new QString("https://status.exosite.com");
    std::vector<QString*> urls1(1);
    urls1[0] = url1;
    PageWindow::parameters *thisSettings1 = new PageWindow::parameters;
    thisSettings1->urls = urls1;
    thisSettings1->display = 0;
    thisSettings1->interval = 10000;
    pageSettings.push_back(thisSettings1);
    // For 1 End

    // For 2 Start
    QString* url2 = new QString("https://portals.exosite.com/views/2219771501/1385257981");
    std::vector<QString*> urls2(1);
    urls2[0] = url2;
    PageWindow::parameters *thisSettings2 = new PageWindow::parameters;
    thisSettings2->urls = urls2;
    thisSettings2->display = 1;
    thisSettings2->interval = 10000;
    pageSettings.push_back(thisSettings2);
    // For 2 End

    std::vector<PageWindow::parameters*>::iterator iter;
    for (iter = pageSettings.begin(); iter != pageSettings.end(); iter++){
        views.push_back(new PageWindow(parent, (*iter)));
    }
}

MainWindow::~MainWindow()
{
}
