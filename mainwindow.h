#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QMainWindow>
#include <QtWebKit>
#include <QDesktopWidget>
#include <QSettings>
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

    std::vector<PageWindow::parameters*> pageSettings;
    std::vector<PageWindow*> views;
};

#endif // MAINWINDOW_H
