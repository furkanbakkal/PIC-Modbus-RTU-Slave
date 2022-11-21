# PIC-Modbus-RTU-Slave
PIC18F4520 Modbus RTU as Slave - Led Blink - Read Write Registers

Project tested with PIC18F4520, compiler CCS and Python 3.7

------------------

## Extra Components for Modbus Communication:
TTL to USB : https://www.robolinkmarket.com/usb-rs485-donusturucu-modul 

RS485 to TTL : https://www.robolinkmarket.com/rs485-ttl-seri-donusturucu-karti

------------------

## Wiring for RS485 Modules and LCD :

DE and RE pins shorted ->D2 

DI -> TX 

RO -> RX 

A->A  

B->B


LCD_D4 -> PIN_B4

LCD_D5 -> PIN_B5

LCD_D6 -> PIN_B6

LCD_D7 -> PIN_B7

LCD_RS  -> PIN_B3

LCD_RW ->  PIN_B2

LCD_E ->  PIN_B1


LED  -> PIN_C4

------------------

## Supported Functions:

3 : Read holding register

6 : Write single register

15: Write multiple registers

------------------

## Serial Port Settings:

Baudrate: 9600 

Data Bit: 8 

Parity Bit: Even

Stop Bit: 1

------------------


