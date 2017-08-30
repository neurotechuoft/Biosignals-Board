# -*- coding: utf-8 -*-
"""
Created on Tue Apr 28 16:57:18 2015

@author: Pranav
"""

import time

import numpy as np

from sklearn.svm import SVR
from sklearn.grid_search import GridSearchCV
#from sklearn.learning_curve import learning_curve
#from sklearn.kernel_ridge import KernelRidge
#import matplotlib.pyplot as plt
from sklearn.metrics import mean_squared_error
from math import sqrt


#rng = np.random.RandomState(0)

#############################################################################
# Generate sample data
#X = 5 * rng.rand(10000, 1)
#y = np.sin(X).ravel()

# Add noise to targets
#y[::5] += 3 * (0.5 - rng.rand(X.shape[0]/5))

#X_plot = np.linspace(0, 5, 100000)[:, None]

#############################################################################
# Fit regression model
train_size = 1280
svr = GridSearchCV(SVR(kernel='rbf', gamma=0.1), cv=5,
                   param_grid={"C": [1e0, 1e1, 1e2, 1e3],
                               "gamma": np.logspace(-2, 2, 5)})

#kr = GridSearchCV(KernelRidge(kernel='rbf', gamma=0.1), cv=5,
#                  param_grid={"alpha": [1e0, 0.1, 1e-2, 1e-3],
#                              "gamma": np.logspace(-2, 2, 5)})

t0 = time.time()
svr.fit(F_NormPycsv, target_A)
svr_fit = time.time() - t0
print("SVR complexity and bandwidth selected and model fitted in %.3f s"
      % svr_fit)

#t0 = time.time()
#kr.fit(X[:train_size], y[:train_size])
#kr_fit = time.time() - t0
#print("KRR complexity and bandwidth selected and model fitted in %.3f s"
#      % kr_fit)

sv_ratio = svr.best_estimator_.support_.shape[0] / train_size
print("Support vector ratio: %.3f" % sv_ratio)

t0 = time.time()
y_svr = svr.predict(testX)
svr_predict = time.time() - t0
rmsA = sqrt(mean_squared_error(test_A, y_svr));

print("SVR prediction for 280 inputs in %.3f s",svr_predict)
print("RMSE for Arousal: %.3f",rmsA)

#t0 = time.time()
#y_kr = kr.predict(X_plot)
#kr_predict = time.time() - t0
#print("KRR prediction for %d inputs in %.3f s"
#      % (X_plot.shape[0], kr_predict))


