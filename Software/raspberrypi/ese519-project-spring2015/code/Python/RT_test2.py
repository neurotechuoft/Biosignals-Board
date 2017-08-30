# -*- coding: utf-8 -*-
"""
Created on Thu Apr 16 22:38:45 2015

@author: Jared
"""

import numpy as np
import pyqtgraph as pg
from pylab import *

pw = pg.plot()


labels = np.loadtxt('C:\Users\Jared\Dropbox\DEAPdatasets\Preprocessed_csv\s01Labels.csv',delimiter=',')
#data = np.loadtxt('C:\Users\Jared\Dropbox\DEAPdatasets\Preprocessed_csv\s01Datav2.csv',delimiter=',',usecols=range(1,8064*20),skiprows=0)
data = np.loadtxt('C:\Users\Jared\Dropbox\DEAPdatasets\Preprocessed_csv\s01Datav2.csv',delimiter=',')

#############################CONSTANTS
ch = array([27, 3, 6, 2, 24, 7, 11, 20]); #channels of interest 4-Front 2-Temp 2-Occ
ch = ch - 1;
cnt = 0;
Fs = 128; #samp/s
viewWin = 4;    #This value is in seconds
###########################################
#init plt
plt.ion()

#f, axarr = plt.subplots(8, sharex=True)
x = [];
t = [];
for i in range(1000):
    print(i)
    t.append(i)
    x.append(data[ch[1],i])

    
    
    pw.plot(t, x, clear=True, pen=(1,3))

    pg.QtGui.QApplication.processEvents()
    #plt.pause(0.00001)
    cnt=cnt+1
    if(cnt>viewWin*Fs):                            #If you have 50 or more points, delete the first one from the array
        x.pop(0)                       #This allows us to just see the last 50 data points
        t.pop(0)
        