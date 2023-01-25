import serial
import codecs
import csv
import time

FILENAME = "kimenet.csv"
ENCODING = 'utf-8'

if __name__ == '__main__':
	with codecs.open(FILENAME, "w", ENCODING) as fp:
		writer = csv.writer(fp)
		writer.writerow(['time, temperature'])
		ser = serial.Serial('/dev/ttyUSB0', 9600, timeout=1)
		ser.flush()
		while True:
			if ser.in_waiting > 0:
				line = ser.readline().decode('utf-8').rstrip()
				print(line)
				writer.writerow([time.time(), line])
