#include "mbed.h"

DigitalOut myled(LED3);
DigitalOut myled2(LED1);

int main()
{
   while (1)
   {
      myled = 1;  // set LED2 pin to high
      myled2 = 0; // set LED1 pin to low
      ThisThread::sleep_for(1s);

      myled.write(0);  // set LED2 pin to low
      myled2.write(0); // set LED1 pin to high
      ThisThread::sleep_for(1s);
      myled.write(1);  // set LED2 pin to low
      myled2.write(0); // set LED1 pin to high
      ThisThread::sleep_for(1s);
      myled.write(0);  // set LED2 pin to low
      myled2.write(0); // set LED1 pin to high
      ThisThread::sleep_for(1s);

      myled.write(1);  // set LED2 pin to low
      myled2.write(0); // set LED1 pin to high
      ThisThread::sleep_for(1s);

      myled.write(0);  // set LED2 pin to low
      myled2.write(0); // set LED1 pin to high
      ThisThread::sleep_for(1s);

      myled.write(0);  // set LED2 pin to low
      myled2.write(1); // set LED1 pin to high
      ThisThread::sleep_for(1s);

      myled.write(0);  // set LED2 pin to low
      myled2.write(0); // set LED1 pin to high
      ThisThread::sleep_for(1s);
      myled.write(0);  // set LED2 pin to low
      myled2.write(1); // set LED1 pin to high
      ThisThread::sleep_for(1s);
      myled.write(0);  // set LED2 pin to low
      myled2.write(0); // set LED1 pin to high
      ThisThread::sleep_for(1s);

   }
}