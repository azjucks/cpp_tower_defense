#pragma once

#include <gp/gp.h>

class Object
{
protected:
    GPVector2     m_pos;
    float         m_height;
    float         m_width;
public:
    GPVector2     getPos() const { return m_pos; };
    float         getHeight() const;
    float         getWidth() const;
};