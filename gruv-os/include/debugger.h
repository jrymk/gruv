#ifndef _DEBUGGER_H_
#define _DEBUGGER_H_

#include <Arduino.h>

namespace debugger
{
    void printBytes(uint8_t *data, uint16_t len);
}

#endif