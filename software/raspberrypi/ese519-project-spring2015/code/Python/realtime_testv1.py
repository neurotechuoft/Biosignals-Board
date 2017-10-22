# -*- coding: utf-8 -*-
"""
Created on Thu Apr 16 20:35:37 2015
This file really just contains a Charlie Foxtrot of random methods for real
time data aquisition
@author: Jared
"""


'''
This particular snippet is very cool as it uses a timer to call the update 
function... could be useful
pw = pg.plot()
timer = pg.QtCore.QTimer()
def update():
    pw.plot(x, y, clear=True)
timer.timeout.connect(update)
timer.start(16)
'''












import matplotlib
matplotlib.use('TkAgg') # <-- THIS MAKES IT FAST!
import matplotlib.pyplot as plt
import numpy as np
import time
from pylab import *
from drawnow import *


def makeFig(): #Create a function that makes our desired plot
    plt.ylim(-15,15)                                 #Set y min and max values
    plt.title('My Live Streaming Sensor Data')      #Plot the title
    plt.grid(True)                                  #Turn the grid on
    plt.ylabel('Voltage')                            #Set ylabels
    plt.plot(x, 'r-', label='CH 1')       #plot the temperature
    plt.legend(loc='upper left')                    #plot the legend


labels = np.loadtxt('C:\Users\Jared\Dropbox\DEAPdatasets\Preprocessed_csv\s01Labels.csv',delimiter=',')
#data = np.loadtxt('C:\Users\Jared\Dropbox\DEAPdatasets\Preprocessed_csv\s01Datav2.csv',delimiter=',',usecols=range(1,8064*20),skiprows=0)
data = np.loadtxt('C:\Users\Jared\Dropbox\DEAPdatasets\Preprocessed_csv\s01Datav2.csv',delimiter=',')

#############################CONSTANTS
ch = array([27, 3, 6, 2, 24, 7, 11, 20]); #channels of interest 4-Front 2-Temp 2-Occ
ch = ch - 1;
cnt = 0;
Fs = 128; #samp/s
viewWin = 10;    #This value is in seconds
###########################################
#init plt
plt.ion()

#f, axarr = plt.subplots(8, sharex=True)
x = [];
for i in range(1000):
    print(i)
    x.append(data[ch[1],i])
    drawnow(makeFig)
    #plt.pause(0.00001)
    cnt=cnt+1
    if(cnt>viewWin*Fs):                            #If you have 50 or more points, delete the first one from the array
        x.pop(0)                       #This allows us to just see the last 50 data points
        
#    plt.scatter(i, y)
#    plt.draw()
#    time.sleep(0.01)
    

# loop to update the data
'''
while True:
    try:
        y[:-10] = y[10:]
        y[-10:] = np.random.randn(10)

        # set the new data
        li.set_ydata(y)

        fig.canvas.draw()

        time.sleep(0.05)
    except KeyboardInterrupt:
        break
'''