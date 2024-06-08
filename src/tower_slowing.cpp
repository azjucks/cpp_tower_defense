#include "tower.h"
#include "enemy.h"

SlowingTower::SlowingTower(GPVector2 pos)
{
    m_pos = pos;
    m_shootTimer = 0.f;
    m_spriteIndex = TowerSprite::E_SLOWING;
    m_range = 96.f;
    m_fireRate = 0.45f;
    m_cost = 80;
    m_slowRate = 0.65f;
    m_damage = 1;
}

void SlowingTower::drawTower(GPLib* gp)
{
    Tower::drawTower(gp);
}

void SlowingTower::updateTower(GPLib* gp, std::vector<Enemy*> enemies, Game& game)
{
    tryToUpgrade(gp, game);
    if (m_shootTimer >= SLOWING_ATK_DELAY)
        m_shootTimer = 0.f;
    m_shootTimer += gpGetFrameTime(gp);
    for (auto e : enemies)
    {
        GPRect rect = {e->getPos().x, e->getPos().y,
                    RENDER_SIZE, RENDER_SIZE};
        if (utils::rectOverlapCircle({m_pos.x + 16, m_pos.y + 16},
                    m_range, rect))
        {
            e->m_slowRate = m_slowRate;
            if (m_shootTimer >= SLOWING_ATK_DELAY)
                e->takeDmg(m_damage);
        }
        else
        {
            e->m_slowRate = 1.f;
        }
    }
}