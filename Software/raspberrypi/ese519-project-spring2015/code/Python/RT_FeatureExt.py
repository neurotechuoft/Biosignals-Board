# -*- coding: utf-8 -*-
"""
Created on Tue Apr 21 21:41:58 2015

This code will be used to continually monitor data and extract features from 
four (4) second windows. 

The data is retrieved from a .dat file that is updated by the Rpi every 
one (1) second with the latest 4 second window.
^This allows for 4 second window with 3 second overlap between windows as in 
the training model

@author: Jared
"""

import threading
import numpy as np
from pylab import *
from matplotlib import pyplot as plt
import scipy.io.wavfile as wav
from numpy.lib import stride_tricks
import struct
import pyqtgraph as pg

############################################
# Define all constants here

alpha = array([10, 12]);
sAlpha = array([8, 10]);
beta = array([12, 27]);
gamma = array([27, 45]);
theta = array([3, 8]);
delta = array([0.2, 3]);

freqBands = array([alpha,
               sAlpha,
                 beta,
                    gamma,
                       theta,
                          delta]);  

winLen = 4;  #Seconds
winDisp = 1; #Seconds
bufferSize = 30;  #number of windows to be added together
fTot = np.zeros([bufferSize,82])  #fTot size (num of windows x num of features)  
##########################################
# Set up spectral graph can comment out
plt = pg.plot()
data = np.zeros(bufferSize)
curve = plt.plot()
plt.setRange(xRange=[0, 64], yRange=[0, 2000])
i = 0

##########################################
# Functions
#used to comput the num of windows
def NumWins(xLen,fs,winLen,winDisp): 
    return round((xLen-(winLen - winDisp)*fs)/(winDisp*fs));

#used to compute line Length    
def LLFn(x): 
    return sum(abs(np.diff(x)));
    
    
#Try this for stft instead of spectrogram
""" short time fourier transform of audio signal """
def stft(sig, frameSize, overlapFac=0.75, window=np.hanning):
    win = window(frameSize)
    hopSize = int(frameSize - np.floor(overlapFac * frameSize))
    
    # zeros at beginning (thus center of 1st window should be for sample nr. 0)
    #samples = np.append(np.zeros(np.floor(frameSize/2.0)), sig)  changed
    samples = sig;
    # cols for windowing
    cols = np.ceil( (len(samples) - frameSize) / float(hopSize)) + 1
    # zeros at end (thus samples can be fully covered by frames)
    samples = np.append(samples, np.zeros(frameSize))
    
    frames = stride_tricks.as_strided(samples, shape=(cols, frameSize), strides=(samples.strides[0]*hopSize, samples.strides[0])).copy()
    frames *= win
    
    return np.fft.rfft(frames)  
###################################################################################################
#THIS IS WHERE THE MAGIC HAPPENS


def getFeats():
    #GET DATA FROM BIN FILE
    f = open('binaryFoo','rb') 
    #tmp = f.read();      

    global data, curve, line, i, fTot
    windows = 1;
    n = 512  # update 10 samples per iteration
            #read from the binary file
    
    data = np.zeros([8, n]);    
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

      
    data[0,:]  = array(ch1); #plot the last 10 values
    data[1,:] = array(ch2); #plot the last 10 values
    data[2,:] = array(ch3); #plot the last 10 values
    data[3,:] = array(ch4); #plot the last 10 values
    data[4,:] = array(ch5); #plot the last 10 values
    data[5,:] = array(ch6); #plot the last 10 values
    data[6,:] = array(ch7); #plot the last 10 values
    data[7,:] = array(ch8); #plot the last 10 values
    
    ##########################################################################
    #COMPUTE FEATURES FOR THE DATA
    ##########################################################################

    #Spectrogram dependent Information
    NFFT = 512;       # the length of the windowing segments
    Fs = 128;  # the sampling frequency
    dt = 1/float(Fs);
    t = arange(0.0, int(data.shape[1])/float(Fs), dt)
        
    totFeats = 82; #total number of features to be used
    
    #initialize arrays for faster computing
    asymFeats = np.empty([windows, (int(freqBands.shape[0])*3)],dtype = float)
    F  = np.empty([windows, totFeats],dtype = float);
    
    for ch in range(0,7):
             
        freqs = np.zeros([257,1])     
        freqs[:,0] = array(range(0,64*4+1))/float(4)
        
        
        Pxx = stft(data[ch,:], winLen*Fs, overlapFac = 0.75, window = np.hanning)
        Pxx = np.transpose(Pxx)
            
        plot = np.append(freqs,abs(Pxx),axis = 1)
        curve.setData(plot)
        
        
        freqs = array(range(0,64*4+1))/float(4)     
        #init freq features vector to be size of ch x number of windows   
        freqFeats = np.empty([int(freqBands.shape[0]), windows],dtype = float)
        #freqFeats2 = np.empty([int(freqBands.shape[0]), windows],dtype = float)
        
            
        for j in range(0,int(freqBands.shape[0])):
            bandInds = np.logical_and(freqs >= freqBands[j,0] , freqs <= freqBands[j,1]);
            #freqFeats[j,:] = np.mean(np.abs(Pxx[bandInds,:]),0);
            freqFeats[j,:] = np.mean(np.abs(Pxx[bandInds,:]),0);
            #end loop
            
        timeavg_bin = np.empty([windows,1]);    
        timeavg_bin[0,0] = np.mean(data[ch,:])
        #timeavg_bin = np.empty([windows,1]);    
        #C = np.convolve(data[ch,:],np.ones(winLen*Fs)/(winLen*Fs),'valid');
        #timeavg_bin[:,0] = C[0:C.size:(winDisp)*Fs];
       
       
        #Time Windowed Features
        LL = np.empty([windows,1]);
        LL[0,0] = LLFn(data[ch,:]);
        
    
                #below this line is matlab code
    #########################################################
       #F,T,O (Frontal, Temporal, and Occipital assymetry index)
       #kept addition row of zeros so could be formated into grid
        if ch == 1:  
            asymFeats[:,0:6] = (np.transpose(freqFeats) - F[:,0:6])/(np.transpose(freqFeats) + F[:,0:6]);
        elif ch == 5:
            asymFeats[:,6:12] =  (np.transpose(freqFeats) - F[:,32:38])/(np.transpose(freqFeats) + F[:,32:38]);
        elif ch == 7:
            asymFeats[:,12:18] =  (np.transpose(freqFeats) - F[:,48:54])/(np.transpose(freqFeats) + F[:,48:54]);
        
        #make feature matrix
        tmp = np.append(np.transpose(freqFeats), (timeavg_bin),axis = 1)
        F[:,(ch*7):(ch*7)+8] = np.append(tmp, LL,axis = 1)
        #end loop

        
    F[:,-18:] = asymFeats;
    
    #add current F to circular F buffer to be averaged
    fTot[i,:] = F;
    i = (i+1) % bufferSize;
    print(i)   
    threading.Timer(1, getFeats).start()

    
"""   
Load baseline featavg and featstd for normalization 
HOW????
    for i in range(0,totFeats):
        F[:,i] = (F[:,i] - featAv[i])/featStd[i]
   """     
i = 0;
getFeats();
