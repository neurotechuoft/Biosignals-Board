import time
import numpy as np
import matplotlib.pyplot as plt

im = plt.imread("valarr.png")
implot = plt.imshow(im)

plt.ion()
plt.show()

i=0;

while 1:
	i = i+1;
	y = np.random.random_integers(0,650,1);
	x = np.random.random_integers(0,650,1);
	#plt.scatter(x=[790-x, 790+x], y=[705-y, 705+y], c='r', s=40)	
	plt.scatter(x=[790+x], y=[705+y], c='r', s=40)	
	plt.draw()
	time.sleep(0.05)
	plt.scatter(x=[790+x], y=[705+y], c='w', s=40, linewidths='0')	
	if i==30:
		i=0
		plt.clf();
		im = plt.imread("valarr.png")
		implot = plt.imshow(im)
		plt.ion()
		plt.show()