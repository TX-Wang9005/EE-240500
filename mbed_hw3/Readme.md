# mbed HW3

### How to program
1. Compile main.cpp 
` sudo mbed compile --source . --source ~/ee2405/mbed-os-build/ -m B_L4S5I_IOT01A -t GCC_ARM --profile tflite.json -f`
2. Go to screen mode
```shell=
sudo screen /dev/ttyACM0
```

3. Waiting for wifi connection
4. Send command to go to Gesture UI mode ( LED 3 will turn on )
```shell=
/mode_c/run 1
```
5. Use gesture to control the angle
6. Press user button to send selected angle to broker
7. Send command to back to RPC loop
```shell=
/mode_c/run 3
```
8. Send command to go to Tilt angle detection mode ( LED 2 will turn on )
```shell=
/mode_c/run 2
```
9. Tilt mbed to get angle, if it is larger than selected angle, it will send message to broker 
10. After 5 times, Tilt angle detection mode will stop

### Result
#### Screen:
![](https://i.imgur.com/Yllr3sr.png)
#### uLCD:
##### Gesture UI mode
![](https://i.imgur.com/dTMdPtU.jpg)
##### Tilt angle detection mode
![](https://i.imgur.com/vQj1kDd.jpg)
