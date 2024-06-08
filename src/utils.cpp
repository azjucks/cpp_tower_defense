#include "utils.h"
#include <cmath>

bool utils::isMouseInsideRect(GPRect rect, GPVector2 mouse)
{
    return (mouse.x >= rect.x && mouse.x < rect.x + rect.w) &&
           (mouse.y >= rect.y && mouse.y < rect.y + rect.h);
}

bool utils::twoRectOverlap(GPRect r1, GPRect r2)
{
    if (r1.x + r1.w >= r2.x &&     // r1 right edge past r2 left
    r1.x <= r2.x + r2.w &&       // r1 left edge past r2 right
    r1.y + r1.h >= r2.y &&       // r1 top edge past r2 bottom
    r1.y <= r2.y + r2.h) {       // r1 bottom edge past r2 top
        return true;
    }
    return false;
}

float vector2Magnitude(GPVector2 vec)
{
	return sqrt(pow(vec.x, 2) + pow(vec.y, 2));;
}

GPVector2 utils::vector2Substract(GPVector2 vec1, GPVector2 vec2)
{
	GPVector2 vecSubstract = {vec1.x - vec2.x, vec1.y - vec2.y};

	return vecSubstract;
}

float vector2Distance(GPVector2 vec1, GPVector2 vec2)
{
	return vector2Magnitude(utils::vector2Substract(vec1, vec2));
}

bool utils::vector2Compare(GPVector2 vec1, GPVector2 vec2, float epsilon)
{
	return (vector2Distance(vec1, vec2) <= epsilon);
}

float utils::length(GPVector2 vec)
{
    return sqrtf(vec.x * vec.x + vec.y * vec.y);
}

GPVector2 utils::normalize(GPVector2 vec)
{
    return {vec.x/length(vec), vec.y/length(vec)};
}

float vector2DotProduct(GPVector2 vec1, GPVector2 vec2)
{
	return vec1.x * vec2.x + vec1.y * vec2.y;
}

float utils::getAngle(GPVector2 vec1, GPVector2 vec2)
{
	/*float div = vector2Magnitude(vec1) * vector2Magnitude(vec2);

	return acosf(vector2DotProduct(vec1, vec2) / div);
    */
    
    GPVector2 vecSub = utils::vector2Substract(vec1, vec2);
    vecSub = utils::normalize(vecSub);
	return atan2f(vecSub.y, vecSub.x);
    
}

bool utils::rectOverlapCircle(GPVector2 cCenter, float cRadius, GPRect rect)
{

    float testX = cCenter.x;
    float testY = cCenter.y;

    if (cCenter.x < rect.x)         testX = rect.x;
    else if (cCenter.x > rect.x + rect.w) testX = rect.x + rect.w;
    if (cCenter.y < rect.y)         testY = rect.y;
    else if (cCenter.y > rect.y + rect.h) testY = rect.y + rect.h;

    float distX = cCenter.x-testX;
    float distY = cCenter.y-testY;
    float distance = sqrt( (distX*distX) + (distY*distY) );

    if (distance <= cRadius) {
        return true;
    }
    return false;
}

bool utils::pointInsideRect(GPVector2 vec, GPRect rect)
{
    if (vec.x >= rect.x &&
        vec.x <= rect.x + rect.w &&
        vec.y >= rect.y &&
        vec.y <= rect.y + rect.h) {
            return true;
    }
    return false;
}