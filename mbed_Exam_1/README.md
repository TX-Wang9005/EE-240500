# mbed_Exam_1
### This Exam contain two source code file

`main.cpp`
`FFT.py`

### How to program

1. Compile main.cpp 
```shell=
sudo mbed compile --source . --source ~/ee2405/mbed-os-build/ -m B_L4S5I_IOT01A -t GCC_ARM -f
```

2. Using button to select frequency
   - one button for up
   - one button for down
   - one burrun for comfirm selection

3. Waiting 6 second for sampling time

4. Compile FFT.py for Fourier Transform
```shell=
sudo python3 FFT.py
```


&ensp; and we get the Fourier Transform graph 
## screen
![image](https://user-images.githubusercontent.com/74907888/113850174-09597400-974f-11eb-9c29-1f8d4b444e7b.png)
## mbed
![image](https://user-images.githubusercontent.com/74907888/113850483-5f2e1c00-974f-11eb-8f8c-9c281ef8f0f1.png)





