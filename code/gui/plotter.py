# borrowed from pyqtgraph
# this uses the first method of implementing scrolling plots
import sys
import pyqtgraph as pg
from pyqtgraph.Qt import QtCore, QtGui
import numpy as np

class CustomWidget(pg.GraphicsWindow):
    pg.setConfigOption('background', 'w')
    pg.setConfigOption('foreground', 'k')
    def __init__(self, parent=None, **kargs):
        pg.GraphicsWindow.__init__(self, **kargs)
        self.setParent(parent)
        self.setWindowTitle('Aww Yeah')
        p1 = self.addPlot(labels = {'left':'Amplitude', 'bottom':'Time'})
        self.data1 = np.random.normal(size=300)
        self.curve1 = p1.plot(self.data1)

        timer = pg.QtCore.QTimer(self)
        timer.timeout.connect(self.update)
        timer.start(50)

    def update(self):
        self.data1[:-1] = self.data1[1:]  # shift data in the array one sample left
                            # (see also: np.roll)
        self.data1[-1] = np.random.normal()
        self.curve1.setData(self.data1)

## Start Qt event loop unless running in interactive mode or using pyside.
if __name__ == '__main__':
    w = CustomWidget()
    w.show()
    QtGui.QApplication.instance().exec_()
