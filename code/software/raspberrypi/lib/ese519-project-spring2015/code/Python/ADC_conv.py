# -*- coding: utf-8 -*-
"""
Created on Mon Apr 20 20:05:21 2015
This function is used to convert the ACD output of the ads1299 into a float 
that can be processed.


@author: Jared
"""

import struct
import numpy as np
import  io
import pyqtgraph as pg
from pylab import *
import time
import random


##############################################################################
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
##############################################################################
#Variable
#make a bunch or random 24 bit values
rand1 = array(random.sample(xrange(0,0xFFFFFF), 1000)); 
#convert the random numbers to be in 24 bit format 
byte24 = [];
x = [];
for i in range(0,rand1.size):
    byte24.append( format(rand1[i], '024b'));
    x.append(adc2float(int(byte24[i],2), 24, 5))
    
    
    


