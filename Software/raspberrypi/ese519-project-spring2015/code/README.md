# Code Structure

- MATLAB : Includes all the matlab scripts for processing raw eeg signals initially for feature and channel selection.

- Python : Contains all the final python scripts for running raw eeg collection, feature selection. 
	- Run ./server in this folder to start the server communication with the client.
	
- raspi : Contains all the raspberry pi scripts used for the project. 
	- raspi/demo : contains the the final code for extracting eeg signals from the brain to the raspberry pi and sending to the computer. 
	
- webapp : Web application for showing the presentation and youtube videos to the user.
	- run ./start to run the server on the localhost at port 9000
	- platform: nodejs (javascript)
