#include "types.h"
#include "gdt.h"

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

    while(1);
}