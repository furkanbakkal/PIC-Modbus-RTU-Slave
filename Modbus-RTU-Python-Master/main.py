import pymodbus #pip install pymodbus
from pymodbus.pdu import ModbusRequest
from pymodbus.client.sync import ModbusSerialClient as ModbusClient
from pymodbus.transaction import ModbusRtuFramer
import time

client = ModbusClient(method='rtu', port="COM3", baudrate=9600, parity='E', timeout=0.05)
connection = client.connect()
slave_id=5

while True:
    
    #  read registers
    read_vals  = client.read_holding_registers(0, 3, unit=slave_id) # start_address, count, slave_id
    print("Registers:",read_vals.registers)

    # write registers
    address_data=int(input("Addess: "))
    value_data=int(input("Value: "))
    write  = client.write_register(address_data,value_data,unit=slave_id) # address, value to set, slave ID 

    time.sleep(0.2)