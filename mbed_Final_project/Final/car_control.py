import serial
import time
import sys,tty,termios

# XBee setting
serdev = '/dev/ttyUSB0'
s = serial.Serial(serdev, 9600, timeout=3)

class _Getch:
    def __call__(self):
        fd = sys.stdin.fileno()
        old_settings = termios.tcgetattr(fd)
        try:
            tty.setraw(sys.stdin.fileno())
            ch = sys.stdin.read(1)
        finally:
            termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)
        return ch

def get():
    inkey = _Getch()
    while(1):
        k=inkey()
        if k!='':break
    if k=='\x1b':
        k2 = inkey()
        k3 = inkey()
        if k3=='A':
            print ("up")
            s.write("/goStraight/run 100 \n".encode())
        if k3=='B':
            print ("down")
            s.write("/goStraight/run -100 \n".encode())
        if k3=='C':
            print ("right")
            s.write("/turn/run 100 -0.1 \n".encode())
        if k3=='D':
            print ("left")
            s.write("/turn/run 100 0.1 \n".encode())
        time.sleep(1)
        s.write("/stop/run \n".encode())
    elif k=='q':
        print ("quit")
        return 0
    else:
        print ("not an arrow key!")
    return 1

print('Start Communication, sending RPC')
send = 'start'
print(send)
s.write("/line_start/run\n".encode())

while send!='stop':
   send = s.readline()
   print(send)
   if (send == b'ping'):
       s.write("/ping_detection/run\n".encode())

s.close()