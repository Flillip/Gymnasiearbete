#include "mouse.h"

void printf(char*);

MouseDriver::MouseDriver(InterruptManager* manager)
: InterruptHandler(0x21, manager),
  dataport(0x60),
  commandport(0x64)
{
    while(commandport.Read() & 0x1)
        dataport.Read();
    
    commandport.Write(0xAE); // activate interrupts
    commandport.Write(0x20); // get current state
    uint8_t status = (dataport.Read() | 1) & ~0x10;
    commandport.Write(0x60); // set state
    dataport.Write(status);

    dataport.Write(0xF4);
}

MouseDriver::~MouseDriver()
{

}

uint32_t MouseDriver::HandleInterrupt(uint32_t esp)
{   
    uint8_t key = dataport.Read();
    

    return esp;
}