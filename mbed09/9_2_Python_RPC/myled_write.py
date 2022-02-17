import serial

import time

# use the device name you get from `ls /dev/ttyACM*`
serdev = '/dev/ttyACM0'

s = serial.Serial(serdev, 9600)


s.write(bytes("/myled/write 1\r", 'UTF-8'))

line = s.readline()  # Read an echo string from mbed terminated with '\n' (putc())

print(line)

line = s.readline()  # Read an echo string from mbed terminated with '\n' (RPC reply)

print(line)

time.sleep(1)


s.write(bytes("/myled/write 0\r", 'UTF-8'))

line = s.readline()  # Read an echo string from mbed terminated with '\n' (putc())

print(line)

line = s.readline()  # Read an echo string from mbed terminated with '\n' (RPC reply)

print(line)

s.close()
