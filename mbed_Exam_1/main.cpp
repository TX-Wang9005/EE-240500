#include "mbed.h"

#include "uLCD_4DGL.h"

uLCD_4DGL uLCD(D1, D0, D2);
DigitalIn but1(D11);
DigitalIn but2(D12);
DigitalIn but3(D13);
EventQueue queue(32 * EVENTS_EVENT_SIZE);
Thread t;
AnalogOut tmpout(PA_4);
AnalogIn Ain(A0);
Thread thread;
Timer tt;
int mode = 0;
int flag = 1;
float ADCout[1000];

void LCD()
{
    uLCD.cls();
    uLCD.textbackground_color(0x0);
    uLCD.background_color(0x0);
    uLCD.printf("\nSelect Slew Rates\n"); //Default Green on black text
    if (mode == 0)
    {
        uLCD.textbackground_color(0xFFFFFF);
        uLCD.printf("1\n");
        uLCD.textbackground_color(0x0);
        uLCD.printf("1/2\n");
        uLCD.printf("1/4\n");
        uLCD.printf("1/8\n");
    }
    else if (mode == 1)
    {
        uLCD.printf("1\n");
        uLCD.textbackground_color(0xFFFFFF);
        uLCD.printf("1/2\n");
        uLCD.textbackground_color(0x0);
        uLCD.printf("1/4\n");
        uLCD.printf("1/8\n");
    }
    else if (mode == 2)
    {
        uLCD.printf("1\n");
        uLCD.printf("1/2\n");
        uLCD.textbackground_color(0xFFFFFF);
        uLCD.printf("1/4\n");
        uLCD.textbackground_color(0x0);
        uLCD.printf("1/8\n");
    }
    else if (mode == 3)
    {
        uLCD.printf("1\n");
        uLCD.printf("1/2\n");
        uLCD.printf("1/4\n");
        uLCD.textbackground_color(0xFFFFFF);
        uLCD.printf("1/8\n");
        uLCD.textbackground_color(0x0);
    }
}
void Add(void)
{
    mode++;
    mode = mode % 4;
    queue.call(LCD);
}

void Minus(void)
{
    mode--;
    mode = (mode + 4) % 4;
    queue.call(LCD);
}

void enter(void)
{
    flag = 0;
}
void sig(void)
{
    if (mode == 0)
    {
        while (1)
        {
            for (int i = 0; i < 80; i += 1)
            {
                tmpout = float(i) / 88;
                ThisThread::sleep_for(1ms);
            }
            ThisThread::sleep_for(80ms);
            for (float i = 80; i > 0; i -= 1)
            {
                tmpout = float(i) / 88;
                ThisThread::sleep_for(1ms);
            }
        }
    }
    else if (mode == 1)
    {
        while (1)
        {
            for (int i = 0; i < 40; i += 1)
            {
                tmpout = float(i) / 44;
                ThisThread::sleep_for(1ms);
            }
            ThisThread::sleep_for(160ms);
            for (float i = 40; i > 0; i -= 1)
            {
                tmpout = float(i) / 44;
                ThisThread::sleep_for(1ms);
            }
        }
    }
    else if (mode == 2)
    {
        while (1)
        {
            for (int i = 0; i < 20; i += 1)
            {
                tmpout = float(i) / 22;
                ThisThread::sleep_for(1ms);
            }
            ThisThread::sleep_for(200ms);
            for (float i = 20; i > 0; i -= 1)
            {
                tmpout = float(i) / 22;
                ThisThread::sleep_for(1ms);
            }
        }
    }
    else
    {
        while (1)
        {
            for (int i = 0; i < 10; i += 1)
            {
                tmpout = float(i) / 11;
                ThisThread::sleep_for(1ms);
            }
            ThisThread::sleep_for(220ms);
            for (float i = 10; i > 0; i -= 1)
            {
                tmpout = float(i) / 11.0;
                ThisThread::sleep_for(1ms);
            }
        }
    }
}

int main()
{
    t.start(callback(&queue, &EventQueue::dispatch_forever));
    LCD();
    while (flag == 1)
    {
        if (but1.read() == 1)
        {
            Add();
        }
        else if (but2.read() == 1)
        {
            Minus();
        }
        else if (but3.read() == 1)
        {
            enter();
        }
        ThisThread::sleep_for(1s);
    }
    thread.start(sig);
    // tt.start();

    for (int i = 0; i < 1000; i++)
    {
        ADCout[i] = Ain;
        ThisThread::sleep_for(1ms);
    }
    // tt.stop();
    // auto ms = chrono::duration_cast<chrono::milliseconds>(tt.elapsed_time()).count();
    // printf("Timer time: %llu ms\n", ms);
    for (int i = 0; i < 1000; i++)
    {
        printf("%f\r\n", ADCout[i]);
    }
}