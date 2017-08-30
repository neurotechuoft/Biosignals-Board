# -*- coding: utf-8 -*-
"""
Created on Tue Apr 21 13:50:55 2015

@author: Jared
"""


## This example demonstrates many of the 2D plotting capabilities
## in pyqtgraph. All of the plots may be panned/scaled by dragging with 
## the left/right mouse buttons. Right click on any plot to show a context menu.

#rom PySide.QtCore import QTime
from pyqtgraph.Qt import QtGui, QtCore
import numpy as np
import pyqtgraph as pg
import sys
import struct
from pylab import *
import time





#QtGui.QApplication.setGraphicsSystem('raster')
#app = QtGui.QApplication([])
#mw = QtGui.QMainWindow()
#mw.resize(800,800)

win = pg.GraphicsWindow(title="Basic plotting examples")
win.resize(1200,800)


#f = open('binaryFoo','rb')       
################################MOCK REAL TIME DATA###########################
#init data variables
bufferSize = 1000
fs = float(128);

data = np.zeros(bufferSize)
data2 = np.zeros(bufferSize)
data3 = np.zeros(bufferSize)
data4 = np.zeros(bufferSize)
data5 = np.zeros(bufferSize)
data6 = np.zeros(bufferSize)
data7 = np.zeros(bufferSize)
data8 = np.zeros(bufferSize)
############################INIT ALL CHANNEL PLOTS############################
#constants to change make it easier
rng = [-50, 50];  


p1 = win.addPlot(col = 0, row = 0)
c1 = p1.plot()
line1 = p1.addLine(x=0)
p1.setRange(xRange=[0, bufferSize], yRange= rng)
p1.showAxis('bottom', False)



p2 = win.addPlot(col = 0, row = 1)
c2 = p2.plot()
line2 = p2.addLine(x=0)
p2.setRange(xRange=[0, bufferSize], yRange= rng)
p2.showAxis('bottom', False)


p3 = win.addPlot(col = 0, row = 2)
c3 = p3.plot()
line3 = p3.addLine(x=0)
p3.setRange(xRange=[0, bufferSize], yRange= rng)
p3.showAxis('bottom', False)

p4 = win.addPlot(col = 0, row = 3)
c4 = p4.plot()
line4 = p4.addLine(x=0)
p4.setRange(xRange=[0, bufferSize], yRange= rng)
p4.showAxis('bottom', False)

p5 = win.addPlot(col = 0, row = 4)
c5 = p5.plot()
line5 = p5.addLine(x=0)
p5.setRange(xRange=[0, bufferSize], yRange= rng)
p5.showAxis('bottom', False)

p6 = win.addPlot(col = 0, row = 5)
c6 = p6.plot()
line6 = p6.addLine(x=0)
p6.setRange(xRange=[0, bufferSize], yRange= rng)
p6.showAxis('bottom', False)

p7 = win.addPlot(col = 0, row = 6)
c7 = p7.plot()
line7 = p7.addLine(x=0)
p7.setRange(xRange=[0, bufferSize], yRange= rng)
p7.showAxis('bottom', False)

p8 = win.addPlot(col = 0, row = 7)
c8 = p8.plot()
line8 = p8.addLine(x=0)
p8.setRange(xRange=[0, bufferSize], yRange= rng)
p8.setLabel('bottom', units = 's')
p8.showAxis('bottom', False)


i = 0


def update():
    f = open('binaryFoo','rb') 
    #tmp = f.read();      

    global data, curve, line, i
    n = 10  # update 10 samples per iteration
            #read from the binary file
   
    #x = np.zeros(n);    
    ch1 = np.zeros(n);
    ch2 = np.zeros(n);
    ch3 = np.zeros(n);
    ch4 = np.zeros(n);
    ch5 = np.zeros(n);
    ch6 = np.zeros(n);
    ch7 = np.zeros(n);
    ch8 = np.zeros(n);

##############################################################################
###########   THIS IS WHAT I WILL NEED TO CHANGE IN ORDER TO GET THE DATA FROM
###########   THE DATA FILE BEING STREAMED FROM THE Rpi.

    for j in range(0,n):
        try:
            msg = struct.unpack('f',f.read(4));
            #if msg != "":
            ch1t = struct.unpack('f',f.read(4));
            ch2t = struct.unpack('f',f.read(4));
            ch3t = struct.unpack('f',f.read(4));
            ch4t = struct.unpack('f',f.read(4));
            ch5t = struct.unpack('f',f.read(4));
            ch6t = struct.unpack('f',f.read(4));
            ch7t = struct.unpack('f',f.read(4));
            ch8t = struct.unpack('f',f.read(4));
                
            ch1[j] = ch1t[0]
            ch2[j] = ch2t[0]
            ch3[j] = ch3t[0]
            ch4[j] = ch4t[0]
            ch5[j] = ch5t[0]
            ch6[j] = ch6t[0]
            ch7[j] = ch7t[0]
            ch8[j] = ch8t[0]
    
        except struct.error:
            print "Status: No Data"

      
    data[i:i+n]  = array(ch1); #plot the last 10 values
    data2[i:i+n] = array(ch2); #plot the last 10 values
    data3[i:i+n] = array(ch3); #plot the last 10 values
    data4[i:i+n] = array(ch4); #plot the last 10 values
    data5[i:i+n] = array(ch5); #plot the last 10 values
    data6[i:i+n] = array(ch6); #plot the last 10 values
    data7[i:i+n] = array(ch7); #plot the last 10 values
    data8[i:i+n] = array(ch8); #plot the last 10 values
    
    
    c1.setData(data)
    c2.setData(data2)
    c3.setData(data3)
    c4.setData(data4)
    c5.setData(data5)
    c6.setData(data6)
    c7.setData(data7)
    c8.setData(data8)
    
    i = (i+n) % bufferSize
    
    line1.setValue(i)
    line2.setValue(i)
    line3.setValue(i)
    line4.setValue(i)
    line5.setValue(i)
    line6.setValue(i)
    line7.setValue(i)
    line8.setValue(i)
    f.close();
##############################################################################

    
timer = pg.QtCore.QTimer()
timer.timeout.connect(update)
timer.start(78.5)

## Start Qt event loop unless running in interactive mode or using pyside.
if __name__ == '__main__':
    import sys
    if (sys.flags.interactive != 1) or not hasattr(QtCore, 'PYQT_VERSION'):
        QtGui.QApplication.instance().exec_()