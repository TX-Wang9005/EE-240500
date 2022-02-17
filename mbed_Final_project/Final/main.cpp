#include "mbed.h"

#include "bbcar.h"

#include "bbcar_rpc.h"

Ticker servo_ticker;
PwmOut pin5(D5), pin6(D6);
BufferedSerial uart(D1, D0); //tx,rx
BBCar car(pin5, pin6, servo_ticker);
BufferedSerial xbee(A1, A0);
DigitalInOut ping(D10);

void ping_detection(Arguments *in, Reply *out);
RPCFunction rpcping(&ping_detection, "ping_detection");

Thread thread;
void xbee_RPC(void);

void line_start(Arguments *in, Reply *out);
RPCFunction Line_start(&line_start, "line_start");

void cl(Arguments *in, Reply *out);
RPCFunction Stop_rpc(&cl, "cl");

Timer t;

int main()
{
    thread.start(xbee_RPC);
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
        //Call the static call method on the RPC class
        RPC::call(buf, outbuf);
        printf("%s\r\n", outbuf);
    }
}

void xbee_RPC(void)
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
        printf("%s\r\n", outbuf);
    }
}

void line_start(Arguments *in, Reply *out)
{
    char buf_uart[5] = "line";
    uart.write(buf_uart, 4);
}

void cl(Arguments *in, Reply *out)
{
    car.stop();

    ThisThread::sleep_for(5s);

    char buf_xbee[5] = "ping";

    xbee.write(buf_xbee, 4);
}

void ping_detection(Arguments *in, Reply *out)
{
    float val;

    ping.output();
    ping = 0;
    wait_us(200);
    ping = 1;
    wait_us(5);
    ping = 0;
    wait_us(5);

    ping.input();
    while (ping.read() == 0)
        ;
    t.start();
    while (ping.read() == 1)
        ;
    val = t.read();
    printf("Ping = %lf\r\n", val * 17700.4f);
    t.stop();
    t.reset();

    ThisThread::sleep_for(10ms);
}