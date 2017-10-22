#Data for Emotion Classification

Source: DEAPData (http://www.eecs.qmul.ac.uk/mmv/datasets/deap/)

Total no. of Participants: 32

Total no. of Trials per participant: 40

Total no. of instances = 1280

No. of channels to be used: 8 (based on paper reviews)

Channels Used: 

-Trial:
	- Each Trial consists of participant reaction to an external stimulus (video/audio).
	- It consists of 3 seconds of baseline and 60 seconds of stimulus.
	- Each trial is used to extract a feature vector for an instance based on amplitude values of EEG signals.

-Feature Vector:
	The following 8 features are extracted for each of the 8 channels:
		- <Feature List with Description>

-Target Labels:
	Each Feature Vector corresponds to 2 continuous target variables: Arousal and Valence
	In a 2D plane, an ordered pair of (Arousal,Valence) map to an 'emotion' quadrant.

