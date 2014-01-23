#!/usr/bin/python
# -*- coding: utf-8 -*-

"""
ZetCode PyQt4 tutorial 

This example shows a QtGui.QSlider widget.

author: Jan Bodnar
website: zetcode.com 
last edited: September 2011
"""

import sys
from PyQt4 import QtGui, QtCore
from PyQt4.QtCore import QTimer

import serial
ser = serial.Serial('COM27', 9600, writeTimeout=1)

class Example(QtGui.QWidget):
    
    def __init__(self):
        super(Example, self).__init__()
        
        self.initUI()
        
    def initUI(self):      

        sld = QtGui.QSlider(QtCore.Qt.Horizontal, self)
        sld.setFocusPolicy(QtCore.Qt.NoFocus)
        sld.setGeometry(30, 40, 400, 100)
        sld.setRange(0, 180)
        sld.valueChanged[int].connect(self.changeValue)
        self.slider = sld
        
        self.label = QtGui.QLabel(self)
        self.label.setPixmap(QtGui.QPixmap('mute.png'))
        self.label.setGeometry(160, 40, 80, 30)
        
        self.setGeometry(300, 300, 280, 170)
        self.setWindowTitle('QtGui.QSlider')
        self.show()
        
        # Create a QTimer
        self.timer = QTimer()
        # Connect it to f
        self.timer.timeout.connect(self.timeout)
        # Call f() every 5 seconds
        self.timer.start(0.1)
        
        self.allow = False
        
    def timeout(self):
        #print "timeout"
        #print self.slider.value()
        self.allow = True
        
    def changeValue(self, value):
        #print value
        if self.allow:
            self.allow = False
            print value
            ser.write("%d\n" % value)
    
        
def main():
    
    app = QtGui.QApplication(sys.argv)
    ex = Example()
    sys.exit(app.exec_())


if __name__ == '__main__':
    main()    