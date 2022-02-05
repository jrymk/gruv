#include "touch.h"

GT911 touch::touchPanel = GT911();
uint8_t touch::touchPanelCount;
GTPoint touch::touchPanelPoints[GT911_POINT_COUNT];

void touch::touchPanelHandler(uint8_t count, GTPoint *points)
{
    touch::touchPanelCount = count;
    memcpy(touchPanelPoints, points, GT911_POINT_DATA_SIZE * count);
}