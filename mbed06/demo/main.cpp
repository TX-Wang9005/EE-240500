#include "mbed.h"

using namespace std::chrono;


Ticker flipper;

DigitalOut led3(LED3);


void flip()

{

   led3 = !led3;

}


int main()

{

   led3 = 1;

   flipper.attach(&flip, 500ms); // the address of the function to be attached (flip) and the interval (2 seconds)



}
