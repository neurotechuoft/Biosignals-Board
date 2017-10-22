# -*- coding: utf-8 -*-
"""
Spyder Editor

This is a temporary script file.
"""
import numpy as np
from sklearn import svm
from sklearn.metrics import mean_squared_error
from math import sqrt

testY = np.load('testY.npy');
testX = np.load('testX.npy');
trainX = np.load('trainX.npy');
trainY = np.load('trainY.npy');




svr_rbfA =  svm.SVR(kernel='rbf', C=1e1, gamma=0.06, tol = 0.005);
svr_rbfV =  svm.SVR(kernel='rbf', C=1e1, gamma=0.06, tol = 0.005);

svrA = svr_rbfA.fit(trainX,trainY[:,0]);
A_rbf_pred = svrA.predict(testX);
svrV = svr_rbfV.fit(trainX,trainY[:,1]);
V_rbf_pred = svrV.predict(testX);


rmsA = sqrt(mean_squared_error(testY[:,0], A_rbf_pred));
rmsV = sqrt(mean_squared_error(testY[:,1], V_rbf_pred));

A_1 = svrA.predict(testX[0,:]);

from sklearn.externals import joblib
joblib.dump(svr_rbfA, 'Arousal.pkl')
joblib.dump(svr_rbfV, 'Valence.pkl')
#import pickle
#s = pickle.dumps(svrA)
