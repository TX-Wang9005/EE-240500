#include "mbed.h"

#include "bbcar.h"

#include "bbcar_rpc.h"

Ticker servo_ticker;

PwmOut pin5(D5), pin6(D6);
BufferedSerial pc(USBTX, USBRX); //tx,rx
BufferedSerial uart(D1, D0);     //tx,rx
BBCar car(pin5, pin6, servo_ticker);
//void angle(Arguments *in, Reply *out);
//RPCFunction rpcangle(&angle, "angle");
DigitalInOut ping(D11);
Timer t;

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
//
// void angle(Arguments *in, Reply *out)
// {
//
// float val;
//
// car.stop();
//
// ping.output();
//
// ping = 0;
// wait_us(200);
//
// ping = 1;
// wait_us(5);
//
// ping = 0;
// wait_us(5);
//
// ping.input();
//
// while (ping.read() == 0)
// ;
//
// t.start();
//
// while (ping.read() == 1)
// ;
//
// val = t.read();
//
// printf("Ping = %lf\r\n", val * 17700.4f);
//
// t.stop();
//
// t.reset();
//
// ThisThread::sleep_for(500ms);
// }