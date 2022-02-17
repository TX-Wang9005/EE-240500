import pyb
import sensor
import image
import time
import math

enable_lens_corr = False # turn on for straighter lines...
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
# we run out of memory if the resolution is much bigger...
sensor.set_framesize(sensor.QQVGA)
sensor.skip_frames(time=2000)
clock = time.clock()


uart = pyb.UART(3, 9600, timeout_char=1000)
uart.init(9600, bits=8, parity=None, stop=1, timeout_char=1000)
send = "initial"

line_detection = False

def uart_detect(s):
    global line_detection

    if(s == b'line'):
        line_detection = True
    elif (s == b'stop'):
        line_detection = False
        uart.write(("/cl/run\n").encode())
    else:
        line_detection = line_detection

while(True):
    send = uart.readline()
    uart_detect(send)
    if(send):
        print(send)
    clock.tick()
    img = sensor.snapshot()

    if enable_lens_corr:
        img.lens_corr(1.8) # for 2.8mm lens...

    if(line_detection):
        for l in img.find_line_segments(merge_distance = 200, max_theta_diff = 5):
            img.draw_line(l.line(), color = (255, 0, 0))
            # Translation units are unknown. Rotation units are in degrees.
            line = (l.x1(),l.y1(),l.x2(),l.y2())
            if (l.length() > 60):
                print("x1: %d, y1: %d, x2: %d, y2: %d" % line)
                if (l.x2() < 60):
                    uart.write(("/turn/run 100 -0.5 \n").encode())
                    print("right")
                elif (l.x2() > 110):
                    uart.write(("/turn/run 100 0.5 \n").encode())
                    print("left")
                elif (abs(l.y1() - l.y2()) < 20 and abs(l.x1() - l.x2()) > 50):
                    print("Detect stop")
                    uart_detect(b'stop')
                else:
                    uart.write(("/goStraight/run 100 \n").encode())
                    print("Go straight")
            time.sleep_ms(100)
            uart.write(("/stop/run \n").encode())
