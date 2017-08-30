# -*- coding: utf-8 -*-
"""
Created on Mon Apr 27 15:40:58 2015

@author: Jared
"""

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
import sklearn
import csv
import sys
from sklearn.externals import joblib
from sklearn import svm
import os


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

featAv  = np.loadtxt('F_NormAvPy.csv',delimiter = ',');
featAv = featAv[:,2]
featStd = np.loadtxt('F_NormStdPy.csv',delimiter = ',');
featStd = featStd[:,2]


a = ["QvD6maGRh7c,iZ9vkd7Rp-g,X0eso4ARXzk,TjkQeBG8r_s,7PoJv4N1Too,inavo37kSOg,51ncDQYxsm8,HQzd3BIEabM,eoaTl7IcFs8,piWMN_VNg3Y,AWiccrTB4LM,3Wi1d0FNgdQ,nb1u7wMKywM,uyUOe7HAkpk,DqpFMqfeltE,p_1-VFflpCI,XiLulP9EErc,zwFS69nA-1w,KmcPeuf5aXo,EkHTsc9PU2A,GfDPPEnI1J4,T8YCSJpF4g4,1Uq3nI11w4g,2SpecRRKRoU,AO9dbmJ_2zU,b9E53eUhEmA,sMf2srQ065E,ZpUYjpKg9KY,7usTgw-7XfM,RbACqaCGVk4,I4UM6goudZY,Ypkv0HeUvTc,I5wt302oph0,FgMn2OJmx3w,6VLO9K-lUII,ZxcCN6BgO_k,QNy4iEqQd1Q,rMEp_5FMXRE,gyUWkQj0Q_U,leohcvmf8kM,WlAHZURxRjY,qmVn6b7DdpA,4uzddaCSwpE,z5rRZdiu1UE,K7Ky5R-vxns,VTN31q357Ys,9lp0IWv8QZY,vVy9Lgpg1m8,7_IKcMl_a9A,eoRiVwFP02s,RQa7SvVCdZk,VMpakpQBaxE,-_pTeDz4Zpk,ZiclTJajivc),E2snP7rGP6g,-9tgyQriews,iEe_eraFWWs,mzgjiPBCsss,S0zMHf7J15g,8xg3vE8Ie_E,t1WAMNIunKc,EVYgRPfC9nQ,J5j-ipGFcko,qT6XCvDUUsU,Zx3m4e45bTo,8ftz06I_j-k,dwXzpTU-NCk,sgzeqwhNTDk,N3-_DbEYe7A,GKcQ2RiETss,6fxI1JsXv1A,KzWVWY5QUzg,dxNX_PRqhCQ,6Ejga4kJUts,AJQ4sg3VCsk,IJkMrl4AG8w,rHaKHy8qSpo,Tu1wAP2Baco,7Twnmhe948A"];
vid_class1 = csv.reader(a, delimiter=',')
vid_class1 = list(vid_class1);

a = ["zLDPhPrr5Ig,O0yoxveh7Tg,HdH8NYsDwWk,uuzNohk5cYw,Y9eXzmVzDR8,ScD62dww0Fw,aFn26ntmSsg,TEVodXzNmPM,mZM-d2qD15E"];
vid_class2 = csv.reader(a, delimiter=',')
vid_class2 = list(vid_class2);

a = ["wVyggTKDcOE,ghPcYqn0p4Y,zHqZmtAD0lQ,xxvw5vrJxos"];
vid_class3 = csv.reader(a, delimiter=',')
vid_class3 = list(vid_class3);

a = ["PT47mdE7qY4,S_j7U4R4aO8,_iEnN9ip1Qk,Z50ZveXL-Ps,cCO-3TzoFJo,-eohHwsplvY,A-BSL5Av89w,Zxe4H0kQpis,ijLKoqN5_EY,cQHJuSZTjkE,hme5jf2Z_ow,WDdleGysNys,EDEEzS7OV2k,G-k19OCq7vE,kxWFyvTg6mc,3orLNBS2ZbU,0mSEhr-K05E,5YObYOHIZkk,15kWlTrpt5k,CoSL_qayMCc,Vg1jyL3cr60"];
vid_class4 = csv.reader(a, delimiter=',')
vid_class4 = list(vid_class4);

# access as vid_class1[0][], vid_class2[0][], vid_class3[0][], vid_class4[0][]



fTot = np.zeros([bufferSize,82])  #fTot size (num of windows x num of features)  
fv = np.load('f.npy');
##########################################
# Set up spectral graph can comment out
spect = pg.plot()
curve = spect.plot()
spect.setRange(xRange=[0, 64], yRange=[0, 2000])
i = 0

##########################################
# Functions
#used to comput te num of windows
def NumWins(xLen,fs,winLen,winDisp): 
	return round((xLen-(winLen - winDisp)*fs)/(winDisp*fs));

#used to compute line Length    
def LLFn(x): 
	return sum(abs(np.diff(x)));
	
	
#Try this for stft instead of spectrogram
""" short time fourier transform of audio signal """
def stft(sig, frameSize, overlapFac=0.75, window=np.hanning):
	win = window(frameSize)
	#print(win);
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
#
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
###################################################################################################
#THIS IS WHERE THE MAGIC HAPPENS


def getFeats():
	#GET DATA FROM BIN FILE
	f = open('input_eeg.txt','rb') 
	#tmp = f.read();      

	global data, curve, line, i, fTot, featAv, featStd,F
	windows = 1;
	n = 512  # update 10 samples per iteration
			#read from the binary file
	
	data = np.zeros([8, n]);    
##############################################################################
###########   THIS IS WHAT I WILL NEED TO CHANGE IN ORDER TO GET THE DATA FROM
###########   THE DATA FILE BEING STREAMED FROM THE rPi.
	tmp = [];
	j = 0;
	lines = f.readlines();

	#for line in f:
	for l in range(0,4):
		#print(l);
		line = lines[l];
		#if j==4:
		#break;
		Vref = 4.5;   #This value needs to be determined 5V right now. 
		tmp[:] = line[1:len(line)-1].split(':');
		tmp[:] = tmp[0:len(tmp)-1];
		#print tmp
		tmpInt = array([int(e) for e in tmp]);
		#print tmpInt
		
		for foo in range(0,tmpInt.size):
			temp = adc2float(tmpInt[foo], 24, Vref)*10.0**3;
			tmpInt[foo] = temp;

		#print(tmpInt);
		size(tmp); 
		tmpInt2 =  np.transpose(np.reshape(tmpInt, (128,8)));        
		data[:,(j*128):(j*128)+128] = tmpInt2;
		j = j+1

		
	
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
				
		Pxx = stft(data[ch,:], winLen*Fs, overlapFac = 0, window = np.hanning)
		Pxx = np.transpose(Pxx)
		# print Pxx;
		if (ch == 6):
			plot = np.append(freqs,abs(Pxx),axis = 1);
			curve.setData(plot);
		
		freqs = array(range(0,64*4+1))/float(4);     
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
		F[:,(ch*8):(ch*8)+8] = np.append(tmp, LL,axis = 1)
		#end loop

		
	F[:,-18:] = asymFeats;
	#normalize
	for pp in range(0,totFeats):
		F[:,pp] = (F[:,pp] - featAv[pp])/featStd[pp]

	#print(F);
	#add current F to circular F buffer to be averaged
	fTot[i,:] = F;
	testF = np.mean(fTot, 0);
	print "cur mean"
	print testF
	i = (i+1) % bufferSize;
	#print(i)   if(yhVa > 0 yhAr >0)  #quadrant one
	threading.Timer(1, getFeats).start()

def makePred():

	global fTot, plotI, im, modelA, modelV, F,testX, j

 	curF = np.mean(fTot, 0);
    
     #print "CURRENT VECTOR TO BE PREDICTED ON"
     #print "CURRENT VECTOR"
     #print curF
 	#make prediction with curF for V & A
	yhVa = modelV.predict(curF)
 	yhAr = modelA.predict(curF)     

 	#yhVa = modelV.predict(curF)
 	#yhAr = modelA.predict(curF)   

 	# yhVa = np.random.random_integers(0,4,1);
 	# yhVa = yhVa[0];
 	# yhAr = np.random.random_integers(0,4,1);
 	print yhVa,yhAr

	implot = plt.imshow(im)
	j = j +1;
	plt.ion()
	plt.show()

	


	plotI = plotI+1;
	y = (yhAr*575)/4.0;
	x = (yhVa*575)/4.0;
	print x,y
	#plt.scatter(x=[790-x, 790+x], y=[705-y, 705+y], c='r', s=40)	
	plt.scatter(x=[790+x], y=[705-y], c='r', s=40)	
	plt.draw()
	plt.scatter(x=[790+x], y=[705-y], c='w', s=40, linewidths='0')	
	if plotI==10:
		plotI=0
		plt.clf();
		im = plt.imread("valarr.png")
		implot = plt.imshow(im)
		plt.ion()
		plt.show()


 	if((yhVa > 0) and (yhAr >0)):  #quadrant one
 		songI = np.random.random_integers(0,len(vid_class1[0]),1);
 		songI = songI[0]-1;
 		song = vid_class1[0][songI];

	elif((yhVa < 0) and (yhAr >0)):  #quadrant two
		songI = np.random.random_integers(0,len(vid_class2[0]),1);
 		songI = songI[0]-1; 		
 		song = vid_class2[0][songI];


	elif(yhVa < 0 and yhAr <0):  #quadrant two
		songI = np.random.random_integers(0,len(vid_class3[0]),1);
 		songI = songI[0]-1;
 		song = vid_class3[0][songI];

	else:
		songI = np.random.random_integers(0,len(vid_class4[0]),1);
 		songI = songI[0]-1;
 		song = vid_class4[0][songI];


 	songFile = open('songSel.txt','w');
 	songFile.write(song);
 	songFile.close();

 	threading.Timer(10, makePred).start()


i = 0;
j = 0;
plotI = 0;
im = plt.imread("valarr.png")
#os.system("./runSVR/runSVR_temp.py");
modelA = joblib.load('./runSVR/Arousal.pkl');
modelV = joblib.load('./runSVR/Valence.pkl');
getFeats();
makePred();
