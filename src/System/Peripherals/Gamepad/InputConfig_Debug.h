#ifdef GAMEPAD_DEBUG
#define DBTDEBUG
#pragma once
#include "System/Utilities/Util.h"
#include "Typedefs.h"

// input config for printing HID data used for debugging
namespace Gamepad {
    const char* DEVICE_NAME = "Xbox Wireless Controller";
    const bool reportMapRequired = true;

    void readInput (
        u16 handle,
        u8* data,
        size_t length,
        InputHandler* inputHandler
    ) {
        BT_LOG("Handle: %d, Length: %d\nData: ", handle, length);
        for (size_t i = 0; i < length; i++) {
            BT_LOG("%02x ", data[i]);
        }
        BT_LOG("\n");
    }
};
#endif