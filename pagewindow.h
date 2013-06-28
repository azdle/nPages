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
    struct parameters{
        unsigned int display;
        unsigned int interval;
        std::vector<QString*> urls;
    };
    explicit PageWindow(QWidget *parent = 0, QVariant = QVariant());
    ~PageWindow();
private:
    QStringList pages;
    QWebView* pageView;
    int pageIndex;
    QTimer* reloadTimer;

private slots:
    void nextPage();
    
signals:
    
public slots:
    
};

#endif // PAGEWINDOW_H
