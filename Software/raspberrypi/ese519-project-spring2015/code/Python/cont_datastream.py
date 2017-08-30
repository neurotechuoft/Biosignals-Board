# -*- coding: utf-8 -*-
"""
Created on Tue Apr 21 19:49:50 2015
This function will update a .txt file every 10/128 seconds with the next 10 
samples simulating a cont. stream of data


@author: Jared
"""

import threading
import numpy as np
import pyqtgraph as pg
import sys
import struct
from pylab import *
import time
import io

####################IMPORTANT UPDATE FUNCTION


def update():
    global data, ch, line, i
    n = 10  # update 10 samples per iteration
    
    
    x = "";
    for j in range(0,n):
        
        x += struct.pack('f',random());  #this is the 24bit status packet
        for j in range(0,ch.size):
            x += struct.pack('f',data[j,i])
        
        #open('binaryFoo','ab').write(x);
        i += 1
        
    target = io.open('binaryFoo','wb')
    target.truncate();
    target.write(x);
    target.close();
    print(i);
    threading.Timer(0.0785, update).start()

    



################################MOCK REAL TIME DATA###########################
#load up the MOCK data
labels = np.loadtxt('C:\Users\Jared\Dropbox\DEAPdatasets\Preprocessed_csv\s03Labels.csv',delimiter=',')
#data = np.loadtxt('C:\Users\Jared\Dropbox\DEAPdatasets\Preprocessed_csv\s01Datav2.csv',delimiter=',',usecols=range(1,8064*20),skiprows=0)
data = np.loadtxt('C:\Users\Jared\Dropbox\DEAPdatasets\Preprocessed_csv\s03Datav2.csv',delimiter=',')

ch = array([27, 3, 6, 2, 24, 7, 11, 20]); #channels of interest 4-Front 2-Temp 2-Occ
ch = ch - 1;
data = data[ch,:]


##############################################################################
#binary file
target = io.open('binaryFoo','wb+')
target.truncate();
target.close();
i = 0;
#start function
update();
#timer = pg.QtCore.QTimer()
#timer.timeout.connect(update)
#timer.start(78.5)


#init the txt for data in
#filename = "data.txt";
#target = open(filename, 'w')
#target.truncate()








