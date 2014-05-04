#pragma once

#include <cmath>
#include <math.h>

namespace alife
{
    const double TWOPI = 6.2831853071795864769252867663;
    const double TWO_PI = 6.2831853071795864769252867663;
    const double PI = 3.1415926535897932384626433832795;
    const double HALF_PI = 1.57079632679489661923;
    const double ONE_FOURTH_PI = PI / 4.0;
    const double TWO_FIFTHS_PI = (PI / 5) * 2.0;
    const double THREE_FOURTHS_PI = ONE_FOURTH_PI * 3.0;
    const double SQUARE_ROOT_ZERO_POINT_FIVE = sqrt(0.5);

    template <class T>
    inline T mod(T x, T y)
    {
        if(0 == y)
        {
            return x;
        }

        return x - y * floor(x / y);
    }

    inline float wrapPosNegPI(float angle)
    {
        return alife::mod<float>(angle + PI, TWOPI) - PI;
    }

    inline float wrapTwoPI(float angle)
    {
        return alife::mod<float>(angle, TWOPI);
    }

    inline float wrap360(float angle)
    {
        return alife::mod<float>(angle, 360);
    }

    inline double linlin(double value, double inMin, double inMax, double outMin, double outMax)
    {
        if(value <= inMin)
            return outMin;
        else if(value >= inMax)
            return outMax;
        else
            return ((value - inMin) / (inMax - inMin)) * (outMax - outMin) + outMin;
    }

    // Map a number from a linear range to an exponential range
    inline float linexp(float val, float inMin, float inMax, float outMin, float outMax)
    {
        if(val <= inMin)
            return outMin;
        else if(val >= inMax)
            return outMax;
        else
            return pow(outMax / outMin, (val - inMin) / (inMax - inMin)) * outMin;
    }

    inline int randomRange(int minValue, int maxValue)
    {
        return minValue + (rand() % (maxValue - minValue + 1));
    }
}
