#include <common/types.h>
#include <gdt.h>
#include <hardwarecommunication/interrupts.h>
#include <hardwarecommunication/pci.h>

#include <drivers/driver.h>
#include <drivers/keyboard.h>
#include <drivers/mouse.h>

using namespace OS::common;
using namespace OS::drivers;
using namespace OS::hardwarecommunication;
using namespace OS;

const uint8_t WIDTH = 80;
const uint8_t HEIGHT = 25;

void ClearScreen() 
{
    static uint16_t* VideoMemory = (uint16_t*)0xb8000;

    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            VideoMemory[WIDTH*y+x] = (VideoMemory[WIDTH*y+x] & 0xFF00) | ' ';
        }
    }
}

void printf(char* str)
{
    static uint16_t* VideoMemory = (uint16_t*)0xb8000;
    static uint8_t x = 0, y = 0;

    for (int i = 0; str[i] != '\0'; i++) 
    {
        switch (str[i])
        {
            case '\n':
                y++;
                x = 0;
                break;
            default:
                VideoMemory[WIDTH * y + x] = (VideoMemory[WIDTH * y + x] & 0xFF00) | str[i];
                x++;
        }

        if (x >= WIDTH) 
        {
            y++;
            x = 0;
        }

        if (y > HEIGHT) 
        {
            ClearScreen();
            
            x = 0;
            y = 0;
        }
    }
}

void printfHex(uint8_t hexValue)
{
    char* txt = "00";
    char* hex = "0123456789ABCDEF";
    txt[0] = hex[(hexValue >> 4) & 0x0F];
    txt[1] = hex[(hexValue) & 0x0F];
    printf(txt);
}

class PrintfKeyboardEventHandler : public KeyboardEventHandler
{
    public:
        void OnKeyDown(char c)
        {
            char* str = " ";
            str[0] = c;
            printf(str);
        }
};

class MouseToConsole : public MouseEventHandler
{
    private:
        int8_t x, y;

    public:
        void OnActivate()
        {
            uint16_t* VideoMemory = (uint16_t*)0xb8000;

            x = 40;
            y = 12;
            VideoMemory[80*12+40] = ((VideoMemory[80*12+40] & 0xF000) >> 4)
                                | ((VideoMemory[80*12+40] & 0x0F00) << 4)
                                |  (VideoMemory[80*12+40] & 0x00FF);
        }

        void OnMouseMove(int xOffset, int yOffset)
        {
            static uint16_t* VideoMemory = (uint16_t*)0xb8000;

            VideoMemory[80*y+x] = ((VideoMemory[80*y+x] & 0x0F00) << 4)
                                | ((VideoMemory[80*y+x] & 0xF000) >> 4)
                                | (VideoMemory[80*y+x] & 0x00FF);

            x += xOffset;
            if (x >= 80) x = 79;
            if (x < 0) x = 0;

            y += yOffset;
            if (y >= 25) y = 24;
            if (y < 0) y = 0;

            VideoMemory[80*y+x] = ((VideoMemory[80*y+x] & 0x0F00) << 4)
                                | ((VideoMemory[80*y+x] & 0xF000) >> 4)
                                | (VideoMemory[80*y+x] & 0x00FF);
        }
};

typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstructors()
{
    for(constructor* i = &start_ctors; i != &end_ctors; i++)
        (*i)();
}

extern "C" void kernelMain(void* multiBootStructure, uint32_t magicNumber)
{
    ClearScreen();
    printf("Goodbye World\n");
    
    GlobalDescriptorTable gdt;
    InterruptManager interrupts(&gdt);

    printf("Initializing Hardware, Stage 1\n");

    DriverManager driverManager;
    
    PrintfKeyboardEventHandler kbhandler; 
    MouseToConsole mouseHandler;

    KeyboardDriver keyboard(&interrupts, &kbhandler);
    MouseDriver mouse(&interrupts, &mouseHandler );

    driverManager.AddDriver(&keyboard);
    driverManager.AddDriver(&mouse);

    PeripheralComponentInterconnectController PCIController;
    PCIController.SelectDrivers(&driverManager);

    printf("Initializing Hardware, Stage 2\n");
    driverManager.ActivateAll();

    printf("Initializing Hardware, Stage 3\n");
    interrupts.Activate();

    while(1);
}