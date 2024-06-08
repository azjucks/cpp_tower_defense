#include "projectile.h"

Projectile::Projectile(GPVector2 pos, Enemy* target, int dmg)
{
    m_dmg = dmg;
    m_pos = pos;
    m_target = target;
    m_spd = 96.f;
}

bool Projectile::collideTarget()
{
    if (m_target == nullptr)
    {
        return true;
    }
    GPRect rect = {m_target->getPos().x, m_target->getPos().y,
                    RENDER_SIZE, RENDER_SIZE};
    if (utils::pointInsideRect(m_pos, rect))
    {
        m_target->takeDmg(m_dmg);
        return true;
    }
    return false;
}

void Projectile::projectileUpdate(GPLib* gp)
{
    if (m_target == nullptr)
        return;
    GPVector2 fixedTargetPos = {m_target->getPos().x + 16,
                        m_target->getPos().y + 16};
    GPVector2 vecSub = utils::vector2Substract(fixedTargetPos, m_pos);
    vecSub = utils::normalize(vecSub);

    m_pos.x += (vecSub.x) * m_spd * gpGetFrameTime(gp);
    m_pos.y += (vecSub.y) * m_spd * gpGetFrameTime(gp);
}

void Projectile::projectileDraw(GPLib* gp)
{
    gpDrawCircleFilled(gp, m_pos, 3, {0.f, 1.f, 0.f, 1.f});
}