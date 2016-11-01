# Biosignals-Board

## Overview

The goal of this porject is to create a biosignals-board capable of picking up EEG from an individual. It will pick up the analog signal, amplify it, and convert it to a digital signal to be transfered to a computer. This signal can then be used to control external applications. The biosignals-board will be created to compete in the NeurotechX competition.

### NeurotechX Competition Specifications

1. [Competition Website] (http://neurotechx.com/studentclubs.html#about "NeurotechX Competition")
2. [Rules Breakdown] (http://neurotechuoft.github.io/Files/NeuroTechX-StudentClubCompetition-Validation.pdf "Rules")  

![alt text](./Resources/CompetitionRubric/DIY.png)

## Project Plan

We will be using the ~~ADS1299~~ NeuroSky brainwave [EEG: TGAM] chip to collect, amplify and digitally encode the signal. Using its onboard UART features, we will transmit the signal over bluetooth to a laptop.

[EEG: TGAM]: ./TGAM/


## Project Progress
* Created circuit schematic for interfacing TGAM chip with Arduino
* Researched possible Bluetooth Low Energy chips for use
* Researched TGAM commonications protocol
* Soldered wire sockets to TGAM chip
* Connecting TGAM circuit
* Debugging TGAM + arduino pairing

## Future Tasks
* ~~Solder wire-sockets to TGAM chip~~
* ~~Acquire bread board for testing~~
* Use electrodes(dry) to attach to TGAM
* Interface with Arduino and set up UART connection
* Determine TGAM framework to calculate voltage back
