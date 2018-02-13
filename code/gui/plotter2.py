# borrowed from pyqtgraph
# this uses the first method of implementing scrolling plots
import sys
import pyqtgraph as pg
from pyqtgraph.Qt import QtCore, QtGui
from plotter import CustomWidget
import numpy as np
import math

class CustomWidget2(pg.GraphicsWindow):
    time = 0
    pg.setConfigOption('background', 'w')
    pg.setConfigOption('foreground', 'k')
    def __init__(self, parent=None, **kargs):
        pg.GraphicsWindow.__init__(self, **kargs)
        self.setParent(parent)
        self.setWindowTitle('Aww Yeah')
        p1 = self.addPlot(labels = {'left':'Amplitude', 'bottom':'Frequency'})
        self.data1 = [0] * 300
        self.curve1 = p1.plot(self.data1)

        #turns on the built in power spectrum
        p1.ctrl.fftCheck.setChecked(True)
        
        time = 0

        timer = pg.QtCore.QTimer(self)
        timer.timeout.connect(self.update)
        timer.start(50)

    #generates signals from summation of sin waves
    def signalGenerator(self, event):
        a1=10
        a2=0.5
        b1=7
        b2=0.1
        c1=5
        c2=0.03
        
        signal = (a1) * np.sin((a2) * (self.time)) + (b1) * np.sin((b2) * (self.time))
        + (c1) * np.sin((c2) * (self.time))
        return signal

    def update(self):
        self.data1[:-1] = self.data1[1:]  # shift data in the array one sample left
        self.data1[-1] = self.signalGenerator(self)
        self.curve1.setData(self.data1)
        self.time += 1

## Start Qt event loop unless running in interactive mode or using pyside.
if __name__ == '__main__':
    w = CustomWidget()
    w.show()
    QtGui.QApplication.instance().exec_()
