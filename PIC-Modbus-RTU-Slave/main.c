// TTL to USB : https://www.robolinkmarket.com/usb-rs485-donusturucu-modul
// RS485 to TTL : https://www.robolinkmarket.com/rs485-ttl-seri-donusturucu-karti
// Source: https://github.com/furkanbakkal

//DE and RE pins shorted ->D2
//DI -> TX
//RO -> RX

//A->A  
//B->B

/*Supported functions:

3 : Read holding register
6 : Write single register
15: Wrtie multiple registers
*/

/* LCD Pins

LCD_DB4   PIN_B4
LCD_DB5   PIN_B5
LCD_DB6   PIN_B6
LCD_DB7   PIN_B7

LCD_RS    PIN_B3
LCD_RW    PIN_B2
LCD_E     PIN_B1
*/

#include <main.h>

#define MODBUS_BUS SERIAL
#define MODBUS_TYPE MODBUS_TYPE_SLAVE //PIC is Slave, PC is Master
#define MODBUS_SERIAL_TYPE MODBUS_RTU //Modbus RTU
#define MODBUS_SERIAL_INT_SOURCE MODBUS_INT_RDA //Default TX RX pins 
#define MODBUS_SERIAL_RX_ENABLE PIN_D2 //Enable pin for response
#define MODBUS_OUR_ADDRESS 10

#define MODBUS_SERIAL_BAUD 9600 //baudrate
#define MODBUS_ADDRESS 0x05 //slave id (default:5)

#include "modbus.c" 
#include "flex_lcd420.c" //using 4x20 LCD
 
#define OUT1   PIN_C4 // if address 0 is 1 this pin will be ON, if address 0 is 0 this pin will be OFF

int16 hold_regs[] = {0x0001,0x0002,0x0003}; //registers
 
int8 swap_bits(int8 c)
{
   return ((c&1)?128:0)|((c&2)?64:0)|((c&4)?32:0)|((c&8)?16:0)|((c&16)?8:0)
          |((c&32)?4:0)|((c&64)?2:0)|((c&128)?1:0);
}

void modbus_getdata(){

      if((modbus_rx.address == MODBUS_ADDRESS))
      {
      output_high(MODBUS_SERIAL_RX_ENABLE);
      
         switch(modbus_rx.func)
         {
            case FUNC_READ_HOLDING_REGISTERS:
               if(modbus_rx.data[0] || modbus_rx.data[2] ||
                  modbus_rx.data[1] >= 8 || modbus_rx.data[3]+modbus_rx.data[1] > 8)
                  modbus_exception_rsp(MODBUS_ADDRESS,modbus_rx.func,ILLEGAL_DATA_ADDRESS);
               else
               {
                  if(modbus_rx.func == FUNC_READ_HOLDING_REGISTERS)
                     modbus_read_holding_registers_rsp(MODBUS_ADDRESS,(modbus_rx.data[3]*2),hold_regs+modbus_rx.data[1]);
               }
               break;

            case FUNC_WRITE_SINGLE_REGISTER:
               if(modbus_rx.data[0] || modbus_rx.data[1] >= 8)
                  modbus_exception_rsp(MODBUS_ADDRESS,modbus_rx.func,ILLEGAL_DATA_ADDRESS);
               else
               {
                  hold_regs[modbus_rx.data[1]] = make16(modbus_rx.data[2],modbus_rx.data[3]);
                  
                  modbus_write_single_register_rsp(MODBUS_ADDRESS,
                               make16(modbus_rx.data[0],modbus_rx.data[1]),
                               make16(modbus_rx.data[2],modbus_rx.data[3]));
                              
               }
               break;
               
            case FUNC_WRITE_MULTIPLE_REGISTERS:
               if(modbus_rx.data[0] || modbus_rx.data[2] ||
                  modbus_rx.data[1] >= 8 || modbus_rx.data[3]+modbus_rx.data[1] > 8)
                  modbus_exception_rsp(MODBUS_ADDRESS,modbus_rx.func,ILLEGAL_DATA_ADDRESS);
               else
               {
                  int i,j;
 
                  for(i=0,j=5; i < modbus_rx.data[4]/2; ++i,j+=2)
                     hold_regs[i] = make16(modbus_rx.data[j],modbus_rx.data[j+1]);
 
                  modbus_write_multiple_registers_rsp(MODBUS_ADDRESS,
                                 make16(modbus_rx.data[0],modbus_rx.data[1]),
                                 make16(modbus_rx.data[2],modbus_rx.data[3]));
 
               }
               break;
            default:    //We don't support the function, so return exception
               modbus_exception_rsp(MODBUS_ADDRESS,modbus_rx.func,ILLEGAL_FUNCTION);
         }
      }
      output_low(MODBUS_SERIAL_RX_ENABLE); 
  }

void main()
{
   
   modbus_init();
   lcd_init();
   delay_ms(100);
   
   lcd_gotoxy(1,1);
   lcd_putc("System Started!");
 
   while(TRUE)
   {
   
   lcd_gotoxy(1,2);
   printf(lcd_putc,"%4LX",hold_regs[0]); //2nd line of LCD shows adress 0 value (register 0)
   
   lcd_gotoxy(1,3);
   printf(lcd_putc,"%4LX",hold_regs[1]); //3th line of LCD shows adress 1 value (register 1)
   
   lcd_gotoxy(1,4);
   printf(lcd_putc,"%4LX",hold_regs[2]); //4th line of LCD shows adress 2 value (register 2)
   
   delay_us(50);
      
   modbus_getdata();
   
      
   while(!modbus_kbhit()){ 
   
   if(hold_regs[0]==1){
      output_high(OUT1);
   }
   
   if(hold_regs[0]==0){
      output_low(OUT1);
   }
   
   }
 
    
}

}

