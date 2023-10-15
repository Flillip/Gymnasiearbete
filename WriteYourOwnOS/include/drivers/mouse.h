#ifndef __OS__DRIVERS__MOUSE_H
#define __OS__DRIVERS__MOUSE_H

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

        class MouseEventHandler
        {
            public:
                MouseEventHandler();
                
                virtual void OnActivate();
                virtual void OnMouseDown(uint8_t button);
                virtual void OnMouseUp(uint8_t button);
                virtual void OnMouseMove(int newX, int newY);
        };

        class MouseDriver : public InterruptHandler, public Driver
        {
            private:
                Port8Bit dataport;
                Port8Bit commandport;

                uint8_t buffer[3];
                uint8_t offset;
                uint8_t buttons;

                MouseEventHandler* handler;

            public:
                MouseDriver(InterruptManager* manager, MouseEventHandler* handler);
                ~MouseDriver();

                virtual uint32_t HandleInterrupt(uint32_t esp);
                virtual void Activate();
        };
    }
}

#endif