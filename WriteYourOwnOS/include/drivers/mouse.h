#ifndef __OS__DRIVERS__MOUSE_H
#define __OS__DRIVERS__MOUSE_H

#include <common/types.h>
#include <hardwarecommunication/interrupts.h>
#include <hardwarecommunication/port.h>
#include <drivers/driver.h>

namespace OS
{
    namespace drivers
    {

        class MouseEventHandler
        {
            public:
                MouseEventHandler();
                
                virtual void OnActivate();
                virtual void OnMouseDown(common::uint8_t button);
                virtual void OnMouseUp(common::uint8_t button);
                virtual void OnMouseMove(int newX, int newY);
        };

        class MouseDriver : public hardwarecommunication::InterruptHandler, public Driver
        {
            private:
                hardwarecommunication::Port8Bit dataport;
                hardwarecommunication::Port8Bit commandport;

                common::uint8_t buffer[3];
                common::uint8_t offset;
                common::uint8_t buttons;

                MouseEventHandler* handler;

            public:
                MouseDriver(hardwarecommunication::InterruptManager* manager, MouseEventHandler* handler);
                ~MouseDriver();

                virtual common::uint32_t HandleInterrupt(common::uint32_t esp);
                virtual void Activate();
        };
    }
}

#endif