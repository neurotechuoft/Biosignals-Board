# Biosignals-Board

## Overview

The goal of this project is to build an EEG (electroencephalogram) signal acquisition pipeline for the "Fixed Challenge" portion of the NeurotechX Student competition taking place on March 18, 2018. The pipeline will consist of a biosignals board that is capable of picking up an analog EEG signal, perform analog to digital conversion and signal processing, and transmit the signal to a distant computer for real-time visualization and other applications.

### NeurotechX Competition Specifications

1. [Competition Website](https://neurotechx.github.io/studentclubs/competition/ "NeurotechX Competition")

![Competition Requirements](./resources/CompetitionRequirements2017.JPG)

## Project Plan
We will be developing during the summer and fall of 2017, targetting project completion by the end of 2017, leaving 3 months for additional testing and building upon the competition-ready project. The project is being organized into three subsections: analog signal acquistion and amplification, analog to digital conversion, and data transmission/software, which will all be developed concurrently. For analog to digital conversion, we will be using the Texas Instruments [ADS1299 ADC chip](./code/digital/README.md) to convert the acquired EEG signal.

## Project Progress
* Developed high-level schematic of entire system
![High-level Schematic](./resources/FixedProject.png) 

## Excellent Resources
* [EEG PocketGuide](https://drive.google.com/file/d/0B4xsbDIeDmLPY3hObm1DVjV1TmM/view?usp=sharing)
