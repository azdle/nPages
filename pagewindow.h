#ifndef PAGEWINDOW_H
#define PAGEWINDOW_H

#include <QDialog>
#include <QApplication>
#include <QDesktopWidget>
#include <QtWebKit>
#include <vector>

class PageWindow : public QDialog
{
    Q_OBJECT
public:
    explicit PageWindow(QWidget *parent = 0, std::vector<QString*> urls = std::vector<QString*>(0), int display = 0, int interval = 60000);

private:
    std::vector<QString*> pages;
    QWebView* pageView;

private slots:
    void nextPage();
    
signals:
    
public slots:
    
};

#endif // PAGEWINDOW_H
