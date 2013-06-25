#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QMainWindow>
#include <QtWebKit>
#include <QDesktopWidget>
#include <vector>
#include <pagewindow.h>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

private:
    PageWindow* views[2];
};

#endif // MAINWINDOW_H
