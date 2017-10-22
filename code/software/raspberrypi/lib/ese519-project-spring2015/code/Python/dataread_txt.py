# -*- coding: utf-8 -*-
"""
Created on Thu Apr 16 23:38:09 2015

@author: Jared
"""

import struct
import numpy as np
import pyqtgraph as pg
from pylab import *
import time

#functions
#def as_float32(self):
#
#    s = self.bitlist
#    return unpack("f",pack("I", bits2int(s)))
#
## Where the bits2int function converts bits to an integer.  
#def bits2int(bits):
#    # You may want to change ::-1 if depending on which bit is assumed
#    # to be most significant. 
#    bits = [int(x) for x in bits[::-1]]
#
#    x = 0
#    for i in range(len(bits)):
#        x += bits[i]*2**i
#    return x

#init the txt for data in
#filename = "data.txt";
#f = open('binaryFoo','rb')
#
#x = [];
#byte = [];
#try:
#    byte = struct.unpack('f',f.read(4))
#    val = byte[0]
#    while byte != "":
#        byte = struct.unpack('f',f.read(4))[0]
#        x.append(val)
#        
#finally:
#    f.close()

f = open('binaryFoo','rb')
################################MOCK REAL TIME DATA###########################
plt = pg.plot()
bufferSize = 1000
data = np.zeros(bufferSize)
curve = plt.plot()
line = plt.addLine(x=0)
plt.setRange(xRange=[0, bufferSize], yRange=[-50, 50])
i = 0


def update():
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
            print "no data"
            
        
    data[i:i+n] = array(ch2); #plot the last 10 values
    curve.setData(data)
    i = (i+n) % bufferSize
    line.setValue(i)
    #this is test stuff
#    rand = np.random.normal(size=n)
#    data[i:i+n] = np.clip(data[i-1] + rand, -50, 50)

    
timer = pg.QtCore.QTimer()
timer.timeout.connect(update)
timer.start(20)

"""

for i in range(1000): 
    target.write(str(data[1,i]))
    target.write("\n")
    time.sleep(0.01)
    print(i);
    
print "Fin"
target.close();
"""


