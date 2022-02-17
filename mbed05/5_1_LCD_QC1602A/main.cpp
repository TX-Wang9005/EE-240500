#include "LCD.h"


int main()

{

      LCD_init();                     // call the initialise function

      display_to_LCD(0x48);           // ‘H’

      display_to_LCD(0x45);           // ‘E’

      display_to_LCD(0x4C);           // ‘L’

      display_to_LCD(0x4C);           // ‘L’

      display_to_LCD(0x4F);           // ‘O’

      //for(char x=0x30;x<=0x39;x++)

      //{

            display_to_LCD(1+0x30);      // display numbers 0-9
            display_to_LCD(0+0x30); 
            display_to_LCD(8+0x30); 
            display_to_LCD(0+0x30); 
            display_to_LCD(6+0x30); 
            display_to_LCD(0+0x30); 
            display_to_LCD(0+0x30); 
            display_to_LCD(3+0x30); 
            display_to_LCD(3+0x30); 


      //}

}