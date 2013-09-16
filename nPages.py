#!/usr/bin/env python

import sys, json
from PyQt4.QtCore import *
from PyQt4.QtGui import *
from PyQt4.QtWebKit import *
from pyonep.alias import Alias

class MainWindow(QMainWindow):

    def __init__(self, *args):
        super(MainWindow, self).__init__()

        #self.systemTrayIcon = QSystemTrayIcon(self)
        #self.systemTrayIcon.setIcon(QIcon.fromTheme("face-smile"))
        #self.systemTrayIcon.setVisible(True)

        quitButton = QPushButton("Quit")
        vbox = QVBoxLayout()
        vbox.addWidget(quitButton)
        centralWidget = QWidget()
        centralWidget.setLayout(vbox)
        self.setCentralWidget(centralWidget)

        QObject.connect(quitButton, SIGNAL("clicked()"), self, SLOT("close()"))
        QObject.connect(quitButton, SIGNAL("clicked()"), self.closeAllPageWindows)

        cik = "0000000000000000000000000000000000000000"

        self.exoconn = Alias(cik)
        self.settings = self.exoconn.read('sws')[0][1]

        self.PageWindows = []

        print(self.settings)

        self.generatePageWindows(json.loads(self.settings))

        #self.systemTrayIcon.showMessage('Running', 'Running in the background.')
        self.hide()

    def generatePageWindows(self, settings):
        self.closeAllPageWindows()
        for this_window in settings:
            self.PageWindows.append(PageWindow(this_window))

    def closeAllPageWindows(self):
        #This may be slightly hacky, but garbage collector will delete all PageWindows
        self.PageWindows = []

class PageWindow(QWebView):
    def __init__(self, settings, *args):
        super(PageWindow, self).__init__()

        self.settings = settings
        self.pageIndex = 0
        self.nextPage()
        self.setGeometry(QApplication.desktop().screenGeometry(self.settings['display']))
        self.showFullScreen()

        self.pageTimer = QTimer()
        QObject.connect(self.pageTimer, SIGNAL("timeout()"),
                        self.nextPage)
        self.pageTimer.start(self.settings['interval'])

    def nextPage(self, url = None):
        if url == None:
            url = self.settings['urls'][self.pageIndex]
            self.pageIndex = self.pageIndex + 1
            if self.pageIndex >= len(self.settings['urls']):
                self.pageIndex = 0

        print("Loading:"+url)
        self.load(QUrl(url))

def main(args):
    app = QApplication(args)
    #pageView = PageWindow()
    #pageView.show()
    win = MainWindow()
    win.show()
    app.connect(app, SIGNAL("lastWindowClosed()"),
                app, SLOT("quit()"))
    app.exec_()
  
if __name__=="__main__":
        main(sys.argv)