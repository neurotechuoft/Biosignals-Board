# -*- coding: utf-8 -*-
"""
Created on Thu Apr 16 23:24:16 2015

@author: Jared
"""

import struct
import numpy as np
import  io
import pyqtgraph as pg
from pylab import *
import time

################################MOCK REAL TIME DATA###########################
#load up the MOCK data
labels = np.loadtxt('C:\Users\Jared\Dropbox\DEAPdatasets\Preprocessed_csv\s01Labels.csv',delimiter=',')
#data = np.loadtxt('C:\Users\Jared\Dropbox\DEAPdatasets\Preprocessed_csv\s01Datav2.csv',delimiter=',',usecols=range(1,8064*20),skiprows=0)
data = np.loadtxt('C:\Users\Jared\Dropbox\DEAPdatasets\Preprocessed_csv\s01Datav2.csv',delimiter=',')

ch = array([27, 3, 6, 2, 24, 7, 11, 20]); #channels of interest 4-Front 2-Temp 2-Occ
ch = ch - 1;

data = data[ch,:]


#init the txt for data in
#filename = "data.txt";
#target = open(filename, 'w')
#target.truncate()

#binary file
target = io.open('binaryFoo','wb+')
target.truncate();
target.close();

track = [];
for i in range(2000):
    x = [];
    x = struct.pack('f',random());  #this is the 24bit status packet
    for j in range(0,ch.size):
        x += struct.pack('f',data[j,i])
    #track.append(struct.pack('f',data[1,i]))  DEBUG PURPOSES
    #target.write(x)

    open('binaryFoo','ab').write(x);
    #time.sleep(0.1)
    #close('binaryFoo');
    
    #target.write(binary(data[1,i]))
    #target.write("\n")
    print(i);
    
print "Fin"
