# mbed_hw4

### How to program
HW1
1. Compile main.cpp 
```shell=
sudo mbed compile --source . --source ~/ee2405/mbed-os-build/ -m B_L4S5I_IOT01A -t GCC_ARM -f
```
2. Execute car_control.py
```shell=
sudo python3 car_control.py
```
3. push energy button
4. press right key and left key to reverse parking
5. If you want to char distance, you can change parameter in car_control.py

HW2
1. Compile main.cpp 
```shell    
sudo mbed compile --source . --source ~/ee2405/mbed-os-build/ -m B_L4S5I_IOT01A -t GCC_ARM -f
```
2. Push energy button
3. Car can follow line

HW3
1. compile main.cpp
```shell=
sudo mbed compile --source . --source ~/ee2405/mbed-os-build/ -m B_L4S5I_IOT01A -t GCC_ARM -f
```
2. Push energy button
3. car can perpendicular to the AprilTag surface.
4. compile Ping ( another folder )
```shell=
sudo mbed compile --source . --source ~/ee2405/mbed-os-build/ -m B_L4S5I_IOT01A -t GCC_ARM -f
```
5. open screen
```shell=
sudo screen /dev/ttyACM0
```

### Result
#### Ask mentor for film