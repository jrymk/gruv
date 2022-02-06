#include "debugger.h"

void debugger::printBytes(uint8_t *data, uint16_t len)
{
    for (int i = 0; i < len; i++)
    {
        Serial.printf("%02X", data[i]);
        if ((i & 0b111) == 0b111)
        {
            /* Serial.printf(" | ");
            for (int j = -7; j <= 0; j++)
                Serial.printf("%01c", data[i - j]);
            */
            Serial.printf("\n");
        }
        else
            Serial.printf(" ");
    }
    Serial.printf("-- total length: %d bytes --\n", len);
}