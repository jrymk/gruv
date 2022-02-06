#ifndef _VEC2_H_
#define _VEC2_H_

#include <Arduino.h>

struct vec2f
{
    float x;
    float y;

    vec2f operator+(const vec2f &r);
    vec2f operator-(const vec2f &r);
    vec2f operator*(float c);
    vec2f operator/(float c);
    bool operator==(const vec2f &r);
    bool operator!=(const vec2f &r);
    float length();
    float distance(const vec2f &r);
};

struct vec2
{
    int16_t x;
    int16_t y;

    vec2 operator+(const vec2 &r);
    vec2 operator-(const vec2 &r);
    vec2 operator*(float c);
    vec2 operator/(float c);
    bool operator==(const vec2 &r);
    bool operator!=(const vec2 &r);
    float length();
    float distance(const vec2 &r);

    vec2f toVec2f();
};

#endif