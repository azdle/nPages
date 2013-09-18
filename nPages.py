#!/usr/bin/env python

import sys, json
from PyQt4.QtCore import *
from PyQt4.QtGui import *
from PyQt4.QtWebKit import *
from pyonep.alias import Alias

class MainWindow(QMainWindow):

    def __init__(self, *args):
        super(MainWindow, self).__init__(*args)

        self.settings = QSettings("Exosite", "nPages")

        ### Setup Tray Icon ###
        self.systemTrayIcon = QSystemTrayIcon(self)
        self.systemTrayIcon.setIcon(QIcon.fromTheme("face-smile"))
        self.systemTrayIcon.setToolTip("nPages")
        self.systemTrayIcon.setVisible(True)
        self.systemTrayIcon.activated.connect(self.handleSystemTrayIconActivation)

        ### Create Elements ###
        self.timeoutBar = QProgressBar()
        self.timeoutTimer = QTimer()

        self.cikInput = QLineEdit(self.settings.value('cik').toString())
        self.aliasInput = QLineEdit(self.settings.value('alias').toString())

        self.cikLabel = QLabel("CIK")
        self.aliasLabel = QLabel("Alias")

        self.saveButton = QPushButton("Save Settings")
        self.continueButton = QPushButton("Use Existing Settings")

        self.saveButtonBox = QHBoxLayout()
        self.formLayout = QFormLayout()
        self.formGroup = QGroupBox("Exosite Settings")

        self.mainLayout = QVBoxLayout()
        self.centralWidget = QWidget()

        ### Setup Elements ###
        self.cikInput.setMinimumSize(QSize(325, 0))

        self.timeoutBar.setTextVisible(False)
        self.timeoutBar.setMinimum(0)
        self.timeoutBar.setMaximum(100)
        self.timeoutBar.setInvertedAppearance(True)
        self.timeoutBar.setValue(100)

        self.timeoutTimer.timeout.connect(self.WindowTimerTick)
        self.timeoutTimer.start(100)
        self.countdown = 100

        self.continueButton.clicked.connect(self.Launch)
        self.saveButton.clicked.connect(self.saveSettingsThenLaunch)

        ### Layout Elements ###
        self.formLayout.addRow(self.cikLabel, self.cikInput)
        self.formLayout.addRow(self.aliasLabel, self.aliasInput)
        self.formGroup.setLayout(self.formLayout)

        self.saveButtonBox.addWidget(self.continueButton)
        self.saveButtonBox.addWidget(self.saveButton)

        self.mainLayout.addWidget(self.formGroup)
        self.mainLayout.addItem(self.saveButtonBox)
        self.mainLayout.addWidget(self.timeoutBar)

        self.centralWidget.setLayout(self.mainLayout)
        self.setCentralWidget(self.centralWidget)

        self.PageWindows = []

        self.show()

    def Launch(self):
        self.hide()
        self.generatePageWindows()

        self.timeoutTimer.stop()
        self.timeoutBar.hide()

    def WindowTimerTick(self):
        if self.cikInput.text().length() != 40 or self.aliasInput.text().length() == 0:
            self.countdown = 100
        elif self.countdown <= 0:
            self.Launch()
        else:
            self.countdown = self.countdown - 1

        self.timeoutBar.setValue(self.countdown)

    def generatePageWindows(self):
        self.closeAllPageWindows()
        for this_window in self.fetchPageSettings():
            self.PageWindows.append(PageWindow(this_window))

    def closeAllPageWindows(self):
        self.PageWindows = []

    def handleSystemTrayIconActivation(self, reason):
        if reason == QSystemTrayIcon.Context:
            pass
        elif reason == QSystemTrayIcon.Trigger:
            self.setVisible(not self.isVisible())

    def fetchPageSettings(self):
        self.exoconn = Alias(str(self.settings.value('cik').toString()))
        settingsString = self.exoconn.read(str(self.settings.value('alias').toString()))[0][1]
        print settingsString
        return json.loads(settingsString)

    def saveSettingsThenLaunch(self):
        self.settings.setValue('cik', self.cikInput.text())
        self.settings.setValue('alias', self.aliasInput.text())

        self.Launch()

class PageWindow(QWebView):
    def __init__(self, settings, *args):
        super(PageWindow, self).__init__(*args)

        self.settings = settings
        self.pageIndex = 0
        self.nextPage()
        self.setGeometry(QApplication.desktop().screenGeometry(self.settings['display']))
        self.showFullScreen()

        self.pageTimer = QTimer()
        self.pageTimer.timeout.connect(self.nextPage)
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
    win = MainWindow()
    app.connect(app, SIGNAL("lastWindowClosed()"),
                app, SLOT("quit()"))
    app.exec_()
  
if __name__=="__main__":
        main(sys.argv)