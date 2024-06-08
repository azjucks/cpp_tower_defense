#pragma once

#include <gp/gp.h>

namespace utils
{
    bool isMouseInsideRect(GPRect rect, GPVector2 mouse);

    bool twoRectOverlap(GPRect rect1, GPRect rect2);

    float length(GPVector2 vec);

    bool vector2Compare(GPVector2 vec1, GPVector2 vec2, float epsilon);

    GPVector2 normalize(GPVector2 vec);

    GPVector2 vector2Substract(GPVector2 vec1, GPVector2 vec2);

    bool rectOverlapCircle(GPVector2 cCenter, float cRadius, GPRect rect);

    float getAngle(GPVector2 vec1, GPVector2 vec2);

    bool pointInsideRect(GPVector2 vec, GPRect rect);
}