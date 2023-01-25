import os
import time

while(1):
	t = time.time()
	os.system("raspivid -t 30000 -w 1280 -h 720 -fps 30 -b 2400000 -p 100,100,1280,720 -hf -vf -a 12 -o" + str(t) + ".mp4")
