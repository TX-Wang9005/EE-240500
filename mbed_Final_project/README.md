# mbed_Final_project
 
 ### How to program
HW1
1. Compile main.cpp 
```shell=
sudo mbed compile --source . --source ~/ee2405/mbed-os-build/ -m B_L4S5I_IOT01A -t GCC_ARM -f
```
2. push energy button
3. Execute car_control.py 
```shell=
sudo python3 car_control.py
```
4. Start command will send from XBee
5. Start line detection mode
6. After find a vertical line, it will stop
7. Ping will send distance to screen

 ### Result
 
 https://drive.google.com/file/d/1_TI3A-fYfV6vtqxaqedqaeDQe29imdQD/view?usp=sharing