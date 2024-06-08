#pragma once

#include <gp/gp.h>

#include "object.h"
#include "enemy.h"

class Projectile : public Object
{
private:
    int     m_dmg;
    float   m_spd;
    Enemy*  m_target;
public:
    Projectile(GPVector2 pos, Enemy* target, int dmg);

    Enemy*  getTarget() { return m_target; }
    void    resetTarget() { m_target = nullptr; }
    bool    hasTarget() { return m_target != nullptr; }
    bool    collideTarget();
    void    projectileDraw(GPLib* gp);
    void    projectileUpdate(GPLib* gp);
};