#include "mbed.h"
#include "mbed_rpc.h"
#include "stm32l475e_iot01_accelero.h"
#include "uLCD_4DGL.h"
InterruptIn btnRecord(USER_BUTTON);
uLCD_4DGL uLCD(D1, D0, D2);
EventQueue queue(32 * EVENTS_EVENT_SIZE);

Thread t;
int16_t pDataXYZ[3] = {0};

int idR[32] = {0};

int indexR = 0;
int num = 0;
void getAcc(Arguments *in, Reply *out);

BufferedSerial pc(USBTX, USBRX);

RPCFunction rpcAcc(&getAcc, "getAcc");

void LCD()
{
    uLCD.cls();
    uLCD.textbackground_color(0x0);
    uLCD.background_color(0x0);
    uLCD.printf("\nGestureID\n"); //Default Green on black text
    uLCD.text_width(4);           //4X size text
    uLCD.text_height(4);
    uLCD.printf("%d", num);
}

void record(void)
{

    BSP_ACCELERO_AccGetXYZ(pDataXYZ);
    printf("%d, %d, %d\n", pDataXYZ[0], pDataXYZ[1], pDataXYZ[2]);
}
void startRecord(void)
{

    printf("---start---\n");
    num++;
    printf("GestureID%d :\n", num);
    LCD();
    idR[indexR++] = queue.call_every(1ms, record);

    indexR = indexR % 32;
}

void stopRecord(void)
{

    printf("---stop---\n");

    for (auto &i : idR)

        queue.cancel(i);
}
int main()
{
    LCD();
    char buf[256], outbuf[256]; //RPC
    FILE *devin = fdopen(&pc, "r");
    FILE *devout = fdopen(&pc, "w");
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

void getAcc(Arguments *in, Reply *out)
{
    printf("Start accelerometer init\n");

    BSP_ACCELERO_Init();

    t.start(callback(&queue, &EventQueue::dispatch_forever));
    btnRecord.fall(queue.event(startRecord));

    btnRecord.rise(queue.event(stopRecord));
}