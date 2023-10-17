#ifndef __OS__HARDWARECOMMUNICATION__PCI_H
#define __OS__HARDWARECOMMUNICATION__PCI_H

#include <hardwarecommunication/port.h>
#include <drivers/driver.h>
#include <common/types.h>
#include <hardwarecommunication/interrupts.h>

namespace OS
{
    namespace hardwarecommunication
    {
        enum BaseAddressRegisterType
        {
            MemoryMapping = 0,
            InputOutput = 1
        };

        class BaseAddressRegister
        {
            public:
                bool prefetchable;
                common::uint8_t* address;
                common::uint32_t size;
                BaseAddressRegisterType type;
        };

        class PeripheralComponentInterconnectDescriptor
        {
            public:
                common::uint32_t portBase;
                common::uint32_t interrupt;

                common::uint16_t bus;
                common::uint16_t device;
                common::uint16_t function;

                common::uint16_t vendor_id;
                common::uint16_t device_id;
                
                common::uint8_t class_id;
                common::uint8_t subclass_id;
                common::uint8_t interface_id;

                common::uint8_t revision;

                PeripheralComponentInterconnectDescriptor();
                ~PeripheralComponentInterconnectDescriptor();
        };

        class PeripheralComponentInterconnectController
        {
            private:
                Port32Bit dataPort;
                Port32Bit commandPort;
            
            public:
                PeripheralComponentInterconnectController();
                ~PeripheralComponentInterconnectController();

                common::uint32_t Read(common::uint16_t bus, common::uint16_t device, common::uint16_t function, common::uint32_t registeroffset);
                void Write(common::uint16_t bus, common::uint16_t device, common::uint16_t function, common::uint32_t registeroffset, common::uint32_t value);
                bool DeviceHasFunctions(common::uint16_t bus, common::uint16_t device);

                void SelectDrivers(drivers::DriverManager* driverManager, hardwarecommunication::InterruptManager* interrupts);
                drivers::Driver* GetDriver(PeripheralComponentInterconnectDescriptor, hardwarecommunication::InterruptManager*);
                PeripheralComponentInterconnectDescriptor GetDeviceDescriptor(common::uint16_t bus, common::uint16_t device, common::uint16_t function);
                BaseAddressRegister GetBaseAddressRegister(common::uint16_t bus, common::uint16_t device, common::uint16_t function, common::uint16_t bar);
        };
    }
}

#endif