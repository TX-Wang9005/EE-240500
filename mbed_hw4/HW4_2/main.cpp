#include "mbed.h"

#include "bbcar.h"

#include "bbcar_rpc.h"

Ticker servo_ticker;

PwmOut pin5(D5), pin6(D6);
//BufferedSerial pc(USBTX, USBRX); //tx,rx
BufferedSerial uart(D1, D0); //tx,rx
BBCar car(pin5, pin6, servo_ticker);
//void line(Arguments *in, Reply *out);
//RPCFunction rpcline(&line, "line");

int main()
{
    uart.set_baud(9600);
    char buf[256], outbuf[256];
    FILE *devin = fdopen(&uart, "r");
    FILE *devout = fdopen(&uart, "w");

    while (1)
    {

        memset(buf, 0, 256);

        for (int i = 0;; i++)
        {
            char recv = fgetc(devin);
            if (recv == '\n')
            {
                printf("\r\n");
                break;
            }
            buf[i] = fputc(recv, devout);
        }
        printf("%s\r\n", outbuf);
        RPC::call(buf, outbuf);
    }
}

// void line(Arguments *in, Reply *out)
// {
//     int length = in->getArg<double>();
//     int x2 = in->getArg<double>();

//     if (length >= 60)
//     {
//         if (x2 < 60)
//         {
//             car.turn(100, -0.3);
//             ThisThread::sleep_for(100ms);
//             car.stop();
//         }
//         else if (x2 > 110)
//         {
//             car.turn(100, 0.3);
//             ThisThread::sleep_for(100ms);
//             car.stop();
//         }
//         else
//         {
//             car.goStraight(100);
//             ThisThread::sleep_for(100ms);
//             car.stop();
//         }
//     }
// }
