#pragma once

#include "object.h"

class Entity : public virtual Object
{
protected:
    float m_maxLife;
    float m_currentLife;
};