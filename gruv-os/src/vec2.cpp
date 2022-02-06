#include "vec2.h"

vec2 vec2::operator+(const vec2 &r)
{
    return {x + r.x, y + r.y};
}

vec2 vec2::operator-(const vec2 &r)
{
    return {x - r.x, y - r.y};
}

vec2 vec2::operator*(float c)
{
    return {x * c, y * c};
}

vec2 vec2::operator/(float c)
{
    return {x / c, y / c};
}

bool vec2::operator==(const vec2 &r)
{
    return x == r.x && y == r.y;
}

bool vec2::operator!=(const vec2 &r)
{
    return x != r.x || y != r.y;
}

float vec2::length()
{
    return sqrt(x * x + y * y);
}

float vec2::distance(const vec2 &r)
{
    return sqrt((r.x - x) * (r.x - x) + (r.y - y) * (r.y - y));
}

vec2f vec2::toVec2f()
{
    return {float(x), float(y)};
}

vec2f vec2f::operator+(const vec2f &r)
{
    return {x + r.x, y + r.y};
}

vec2f vec2f::operator-(const vec2f &r)
{
    return {x - r.x, y - r.y};
}

vec2f vec2f::operator*(float c)
{
    return {x * c, y * c};
}

vec2f vec2f::operator/(float c)
{
    return {x / c, y / c};
}

bool vec2f::operator==(const vec2f &r)
{
    return x == r.x && y == r.y;
}

bool vec2f::operator!=(const vec2f &r)
{
    return x != r.x || y != r.y;
}

float vec2f::length()
{
    return sqrt(x * x + y * y);
}

float vec2f::distance(const vec2f &r)
{
    return sqrt((r.x - x) * (r.x - x) + (r.y - y) * (r.y - y));
}