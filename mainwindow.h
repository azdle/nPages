#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QMainWindow>
#include <QtWebKit>
#include <QDesktopWidget>
#include <QDialog>
#include <vector>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

private slots:
    void nextPage();

private:
    QWebView* views[2];
};

#endif // MAINWINDOW_H
