#include <drivers/mouse.h>

using namespace OS::common;
using namespace OS::hardwarecommunication;
using namespace OS::drivers;

void printf(char*);

MouseEventHandler::MouseEventHandler()
{
}

void MouseEventHandler::OnActivate()
{
}

void MouseEventHandler::OnMouseDown(uint8_t button)
{
}

void MouseEventHandler::OnMouseUp(uint8_t button)
{
}

void MouseEventHandler::OnMouseMove(int x, int y)
{
}

MouseDriver::MouseDriver(InterruptManager* manager, MouseEventHandler* handler)
: InterruptHandler(0x2C, manager),
  dataport(0x60),
  commandport(0x64)
{
    this->handler = handler;
}

MouseDriver::~MouseDriver()
{

}

void MouseDriver::Activate()
{
    while(commandport.Read() & 0x1)
        dataport.Read();
    
    offset = 0;
    buttons = 0;

    if (handler != 0)
        handler->OnActivate();
    
    commandport.Write(0xA8); // activate interrupts
    commandport.Write(0x20); // get current state
    uint8_t status = dataport.Read() | 2;
    commandport.Write(0x60); // set state
    dataport.Write(status);

    commandport.Write(0xD4);
    dataport.Write(0xF4);
    dataport.Read();
}

uint8_t custom_floor(float a);

uint32_t MouseDriver::HandleInterrupt(uint32_t esp)
{   
    uint8_t status = commandport.Read();
    if ((!(status & 0x20)) || handler == 0)
        return esp;

    buffer[offset] = dataport.Read();
    offset = (offset + 1) % 3;

    if (offset == 0)
    {
        if (buffer[1] == 0 && buffer[2] == 0)
            return esp;
        
        handler->OnMouseMove(buffer[1], -buffer[2]);
        

        for (uint8_t i = 0; i < 3; i++)
        {
            if ((buffer[0] & (0x01 << i)) != buttons & (0x01 << i))
            {
                if (buttons & (0x1 << i))
                    handler->OnMouseUp(i+1);
                else
                    handler->OnMouseDown(i+1);
            }
        }

        buttons = buffer[0];
    }

    return esp;
}