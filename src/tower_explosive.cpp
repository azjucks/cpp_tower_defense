#include "tower.h"


ExplosiveTower::ExplosiveTower(GPVector2 pos)
{
    m_pos = pos;
    m_shootTimer = 0.f;
    m_spriteIndex = TowerSprite::E_EXPLOSIVE;
    m_attackRadius = 1.3f * RENDER_SIZE;
    m_range = 115.f;
    m_fireRate = 0.85f;
    m_cost = 225;
    m_damage = 7;
}

void ExplosiveTower::drawTower(GPLib* gp)
{
    Tower::drawTower(gp);
    for (auto p : m_projectiles)
    {
        gpDrawCircleFilled(gp, p.getPos(), m_attackRadius,
                {1.f, 0.f, 0.f, 0.15f});
    }
}

void ExplosiveTower::updateTower(GPLib* gp, std::vector<Enemy*> enemies, Game& game)
{
    tryToUpgrade(gp, game);
    for (int i = 0; i < m_projectiles.size(); ++i)
    {
        if (m_projectiles[i].collideTarget())
            {
                for (auto e : enemies)
                {
                    if (e != m_projectiles[i].getTarget())
                    {
                        GPRect rect = {e->getPos().x, e->getPos().y,
                                        RENDER_SIZE, RENDER_SIZE};
                        GPVector2 pPos = {m_projectiles[i].getPos().x + 16,
                                        m_projectiles[i].getPos().y + 16};
                        if (utils::rectOverlapCircle(pPos,
                                m_attackRadius, rect))
                            {
                                e->takeDmg(m_damage);
                            }
                    }
                }
            }
    }
    Tower::updateTower(gp, enemies, game);
}