#ifndef _TOUCH_H_
#define _TOUCH_H_

#include <GT911.h>

namespace touch
{
    extern GT911 touchPanel;
    extern uint8_t touchPanelCount;
    extern GTPoint touchPanelPoints[GT911_POINT_COUNT];
    void touchPanelHandler(uint8_t count, GTPoint *points);
}

#endif