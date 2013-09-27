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
        self.settingsUpdateTime = 0

        ### Setup Tray Menu ###
        self.systemTrayMenu = QMenu(self)
        self.systemTrayMenu.addAction("Settings", self.show)
        self.systemTrayMenu.addAction("Close All Windows", self.closeAllPageWindows)
        self.systemTrayMenu.addAction("Quit", app.quit, "Ctrl+q")

        ### Setup Tray Icon ###
        self.systemTrayIcon = QSystemTrayIcon(self)
        self.systemTrayIcon.setIcon(QIcon.fromTheme("face-smile"))
        self.systemTrayIcon.setToolTip("nPages")
        self.systemTrayIcon.setVisible(True)
        self.systemTrayIcon.activated.connect(self.handleSystemTrayIconActivation)
        self.systemTrayIcon.setContextMenu(self.systemTrayMenu)

        ### Setup Update Timer ###
        self.updateTimer = QTimer()
        self.updateTimer.start(60000) #Check every minute, make this configurable.
        self.updateTimer.timeout.connect(self.checkForUpdates)

        ### Create Elements ###
        self.timeoutBar = QProgressBar()
        self.timeoutTimer = QTimer()
        # There seems to be a bug in QWebView where pages get really slow/stop loading after
        # awhile. Workaround to close windows and open new ones.
        self.restartWorkaroundTimer = QTimer()

        self.cikInput = QLineEdit(self.settings.value('cik').toString())
        self.aliasInput = QLineEdit(self.settings.value('alias').toString())

        self.cikLabel = QLabel("CIK")
        self.aliasLabel = QLabel("Alias")

        self.saveButton = QPushButton("Save Settings")
        self.continueButton = QPushButton("Use Existing Settings")
        self.quitButton = QPushButton("Quit Application")

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

        self.restartWorkaroundTimer.timeout.connect(self.ReLaunch)
        self.restartWorkaroundTimer.start(3600000)

        self.continueButton.clicked.connect(self.Launch)
        self.saveButton.clicked.connect(self.saveSettingsThenLaunch)
        self.quitButton.clicked.connect(app.quit)

        ### Layout Elements ###
        self.formLayout.addRow(self.cikLabel, self.cikInput)
        self.formLayout.addRow(self.aliasLabel, self.aliasInput)
        self.formGroup.setLayout(self.formLayout)

        self.saveButtonBox.addWidget(self.continueButton)
        self.saveButtonBox.addWidget(self.saveButton)

        self.mainLayout.addWidget(self.formGroup)
        self.mainLayout.addWidget(self.timeoutBar)
        self.mainLayout.addItem(self.saveButtonBox)
        self.mainLayout.addWidget(self.quitButton)

        self.centralWidget.setLayout(self.mainLayout)
        self.setCentralWidget(self.centralWidget)

        self.PageWindows = []

        self.show()

    def Launch(self):
        time, settings = self.fetchPageSettings()

        self.settingsUpdateTime = time

        self.hide()
        self.generatePageWindows(settings)

        self.timeoutTimer.stop()
        self.timeoutBar.hide()

    def ReLaunch(self):
        time, settings = self.fetchPageSettings()

        self.settingsUpdateTime = time

        self.generatePageWindows(settings)

    def WindowTimerTick(self):
        if self.cikInput.text().length() != 40 or self.aliasInput.text().length() == 0:
            self.countdown = 100
        elif self.countdown <= 0:
            self.Launch()
        else:
            self.countdown = self.countdown - 1

        self.timeoutBar.setValue(self.countdown)

    def checkForUpdates(self):
        time, settings = self.fetchPageSettings()

        if time > self.settingsUpdateTime:
            print "Updaing"
            self.settingsUpdateTime = time
            self.show() #Hack
            self.closeAllPageWindows()
            self.generatePageWindows(settings)
            self.hide() #Hack
        else:
            print "Not Updating"



    def generatePageWindows(self, settings):
        self.closeAllPageWindows()
        for this_window in settings:
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
        settingsTime, settingsString = self.exoconn.read(str(self.settings.value('alias').toString()))[0]
        return int(settingsTime), json.loads(settingsString)

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
  
if __name__=="__main__":
    app = QApplication(sys.argv)
    win = MainWindow()
    app.exec_()
