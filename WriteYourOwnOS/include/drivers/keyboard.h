#ifndef __OS__DRIVERS__KEYBOARD_H
#define __OS__DRIVERS__KEYBOARD_H

#include <common/types.h>
#include <hardwarecommunication/interrupts.h>
#include <hardwarecommunication/port.h>
#include <drivers/driver.h>

using namespace OS::common;
using namespace OS::hardwarecommunication;
using namespace OS::drivers;

namespace OS
{
    namespace drivers
    {
        class KeyboardEventHandler
        {
            public:
                KeyboardEventHandler();

                virtual void OnKeyDown(char);
                virtual void OnKeyUp(char);
        };

        class KeyboardDriver : public InterruptHandler, public Driver
        {
            private:
                Port8Bit dataport;
                Port8Bit commandport;

                KeyboardEventHandler* handler;

            public:
                KeyboardDriver(InterruptManager* manager, KeyboardEventHandler* handler);
                ~KeyboardDriver();

                virtual uint32_t HandleInterrupt(uint32_t esp);
                virtual void Activate();
        };
    }
}

#endif