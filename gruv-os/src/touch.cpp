#include "touch.h"

GT911 touch::touchPanel = GT911();
uint8_t touch::touchPanelCount = 0;
GTPoint touch::touchPanelPoints[GT911_POINT_COUNT];
uint32_t touch::touchPanelUpdateTimestamp = 0;
bool touch::touchPanelUpdate = false;

touch::TrackPoint touch::trackPoints[TOUCH_MAX_POINTS];
touch::TrackPoint touch::trackPointsPrev[TOUCH_MAX_POINTS];
uint32_t touch::trackTimeDelta = 0;
std::queue<touch::TrackEvent> touch::trackEvents;

void touch::touchPanelHandler(uint8_t count, GTPoint *points)
{
    touchPanelCount = count;
    memcpy(touchPanelPoints, points, GT911_POINT_DATA_SIZE * count);
    touchPanelUpdateTimestamp = millis();
    touchPanelUpdate = true;
}

void touch::update()
{
    touch::touchPanel.loop();

    if (touchPanelUpdate)
    {
        touchPanelUpdate = false;
        trackTimeDelta = millis() - touchPanelUpdateTimestamp;
        touchPanelUpdateTimestamp = millis();

        memcpy(trackPointsPrev, trackPoints, sizeof(trackPointsPrev));

        for (uint8_t i = 0; i < TOUCH_MAX_POINTS; i++)
            trackPoints[i].enabled = false;
        for (uint8_t i = 0; i < touchPanelCount; i++)
        {
            trackPoints[touchPanelPoints[i].trackId].enabled = true;
            trackPoints[touchPanelPoints[i].trackId].pos.x = touchPanelPoints[i].x;
            trackPoints[touchPanelPoints[i].trackId].pos.y = touchPanelPoints[i].y;
            trackPoints[touchPanelPoints[i].trackId].size = touchPanelPoints[i].size;
        }

        // scan for press/releases
        for (uint8_t i = 0; i < TOUCH_MAX_POINTS; i++)
        {
            TrackPoint *tp = &trackPoints[i];
            if (tp->enabled && !trackPointsPrev[i].enabled)
            {
                trackEvents.push(TrackEvent(TrackEventType::PRESS, i));
                tp->pressPos = tp->pos;
                tp->pressTime = millis();
            }

            if (!tp->enabled && trackPointsPrev[i].enabled)
            {
                trackEvents.push(TrackEvent(TrackEventType::RELEASE, i));
                tp->releasePos = trackPointsPrev[i].pos;
                tp->releaseTime = millis();
            }

            if (tp->pos != trackPointsPrev[i].pos)
                trackEvents.push(TrackEvent(TrackEventType::POSITION, i));

            if (tp->size != trackPointsPrev[i].size)
                trackEvents.push(TrackEvent(TrackEventType::SIZE, i));

            if (tp->enabled)
            {
                tp->travel = tp->pos - tp->pressPos;
                tp->travelAngle = acos(tp->travel.x / tp->travel.length()) + (tp->travel.y < 0 ? PI : 0.);
                tp->travelVelocity = tp->travel.toVec2f() / float(millis() - tp->pressTime);
                if (trackPointsPrev[i].enabled)
                {
                    tp->delta = tp->pos - trackPointsPrev[i].pos;
                    tp->deltaAngle = acos(tp->delta.x / tp->delta.length()) + (tp->delta.y < 0 ? PI : 0.);
                    tp->deltaVelocity = tp->delta.toVec2f() / float(trackTimeDelta);
                }
            }
        }
    }
}