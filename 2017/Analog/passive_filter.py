import numpy as np
import matplotlib.pyplot as plt
import sys

# finding the transfer function of a staged series and shunt filter
# transfer = Vout/Vin
# use <Flase> for missing components
def transfer (frequency, R_series, L_series, C_series, R_shunt, L_shunt, C_shunt):
    s = np.pi*2j*frequency
    # using the laplace transform to find complex impedence
    # Z_R = R | Z_C = 1/(sC) | Z_L = sL
    Z_series = np.zeros(len(frequency), dtype = 'complex128')
    one_over_Z_shunt = np.zeros(len(frequency),  dtype = 'complex128')

    # Series
    if R_series:
        Z_series += R_series
    if L_series:
        Z_series += s*L_series
    if C_series:
        Z_series += 1/(s*C_series)

    if C_shunt:
        one_over_Z_shunt += s*C_shunt
    if R_shunt:
        one_over_Z_shunt += 1/R_shunt
    if L_shunt:
        one_over_Z_shunt += 1/(s*L_shunt)

    Z_shunt = 1/one_over_Z_shunt

    # performing voltage division to find transfer function
    return Z_shunt/(Z_shunt+Z_series)

# Decibels are the most common method of visualizing gain
def dB(value):
    return 20*np.log10(value)

# finding the magnitude of complex values for plotting
def mag_dB(value):
    return dB(np.abs(value))

def plot_dB(frequency, gain):
    plt.figure()
    plt.plot(frequency, mag_dB(gain))
    plt.xscale('log')
    plt.yscale('linear')
    plt.grid(True)
    plt.show()

R1 = 100
C1 = 1e-6
L1 = 47e-3
# Creating frequency sweep space
f = np.logspace (-1, 6, 1000)
# f = 100
lowpass = transfer(f, R1, False, False, False, False, C1)
plot_dB(f,lowpass)

highpass = transfer(f, R1, False, C1,False, L1 , False)
plot_dB(f,highpass)

bandpass = lowpass * highpass
plot_dB(f,bandpass)