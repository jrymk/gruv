#ifndef _TOUCH_H_
#define _TOUCH_H_

#include <GT911.h>
#include <queue>
#include <vec2.h>

#define TOUCH_MAX_POINTS 5

namespace touch
{
    extern GT911 touchPanel;
    extern uint8_t touchPanelCount;
    extern GTPoint touchPanelPoints[GT911_POINT_COUNT];
    extern uint32_t touchPanelUpdateTimestamp;
    extern bool touchPanelUpdate;
    void touchPanelHandler(uint8_t count, GTPoint *points);
    void update();

    struct TrackPoint
    {
        bool enabled;
        vec2 pos;
        uint16_t size;
        vec2 pressPos;
        uint32_t pressTime;
        vec2 releasePos;
        uint32_t releaseTime;
        vec2 travel;
        float travelAngle;
        vec2f travelVelocity;
        vec2 delta;
        float deltaAngle;
        vec2f deltaVelocity;
    };

    extern TrackPoint trackPoints[TOUCH_MAX_POINTS];
    extern TrackPoint trackPointsPrev[TOUCH_MAX_POINTS];
    extern uint32_t trackTimeDelta;

    enum TrackEventType
    {
        POSITION,
        SIZE,
        PRESS,
        RELEASE,
    };

    struct TrackEvent
    {
        TrackEventType type;
        uint8_t trackId;

        TrackEvent(TrackEventType _type, uint8_t _trackId) : type(_type), trackId(_trackId) {}
    };
    extern std::queue<TrackEvent> trackEvents;
}

#endif