# -*- coding: utf-8 -*-
"""
Created on Tue Apr 21 19:49:50 2015
This function will update a .txt file every 1 second
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
# This is where the magic happens
#input val -  integer value of 24 bits rep
#input bits - number of bits in val 
#input Vref - This is the value that scales the ACD value

def adc2float(val, bits,Vref):
    """compute the 2's compliment of int value val"""
    if (val & (1 << (bits - 1))) != 0: # if sign bit is set e.g., 8bit: 128-255
        val = val - (1 << bits)        # compute negative value
    
    #Scale val by Vref
    val = float(val)
    val = Vref * (val/(2**(bits-1) + 1))
    
    return val
############################################################################

def float2adc(val,bits,Vref):
            
    val = val/Vref * (2**(bits-1) + 1);
    val = int(val)
    
    if(val < 0):
        val = val + (1 << bits)        # compute negative value
    return val;

##############################################################################

def update():
    global data, ch, line, i
    n = 128  # update 10 samples per iteration
    
    
    x = "";
    for j in range(0,n):
        
        for chan in range(0,ch.size):
            adcVal = float2adc(data[chan,i]*10**-3,24,4.5)  #convert value to uV then to int
            x += str(adcVal) + ";"
        #open('binaryFoo','ab').write(x);
        i += 1
        
    target = io.open('eeg.txt','wb')
    target.truncate();
    target.write(x+"\n");
    target.close();
    #i -= 384
    print(i);
    threading.Timer(1, update).start()

################################MOCK REAL TIME DATA###########################
#load up the MOCK data change this location
data = np.loadtxt('C:\Users\Jared\Dropbox\DEAPdatasets\Preprocessed_csv\s03Datav2.csv',delimiter=',')

ch = array([27, 3, 6, 2, 24, 7, 11, 20]); #channels of interest 4-Front 2-Temp 2-Occ
ch = ch - 1;
data = data[ch,:]

target = io.open('eeg.txt','wb+')
target.truncate();
target.close();
i = 0;
#start function
update();

