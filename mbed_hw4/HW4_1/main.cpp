#include "mbed.h"

#include "bbcar.h"

#include "bbcar_rpc.h"

Ticker servo_ticker;

PwmOut pin5(D5), pin6(D6);

BufferedSerial xbee(D1, D0);

BBCar car(pin5, pin6, servo_ticker);

void back(Arguments *in, Reply *out);
RPCFunction rpcback(&back, "back");

int main()
{

    char buf[256], outbuf[256];

    FILE *devin = fdopen(&xbee, "r");

    FILE *devout = fdopen(&xbee, "w");

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

        RPC::call(buf, outbuf);
    }
}

void back(Arguments *in, Reply *out)
{
    int locate = in->getArg<int>();

    // locate = 0 -> north
    if (locate == 0) // west
    {
        int d1 = in->getArg<int>();
        int d2 = in->getArg<int>();
        car.goStraight(-100);
        ThisThread::sleep_for(1000 * d1);
        car.turn(-100, 0.3);
        ThisThread::sleep_for(1500ms);
        car.goStraight(-100);
        ThisThread::sleep_for(1000 * d2);
    }
    else if (locate == 1) // south
    {
        int d1 = in->getArg<int>();
        car.goStraight(-100);
        ThisThread::sleep_for(1000 * d1);
    }
    else if (locate == 2) // east
    {
        int d1 = in->getArg<int>();
        int d2 = in->getArg<int>();
        car.goStraight(-100);
        ThisThread::sleep_for(1000 * d1);
        car.turn(-100, -0.3);
        ThisThread::sleep_for(1000ms);
        car.goStraight(-100);
        ThisThread::sleep_for(1000 * d2);
    }
    else
        printf("error direction");
}