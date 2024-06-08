#include "enemy.h"
#include <string>


HealerEnemy::HealerEnemy()
{
    m_maxLife = 30;
    m_currentLife = m_maxLife;
    m_direction = EnemySprite::E_DOWN;
    m_currentCell = 0;
    m_waveCost = 3;
    m_healRadius = 2.f * RENDER_SIZE;
    m_healAmount = 2.5f;
    m_healTimer = 0.f;
    m_healDelay = HEAL_DELAY;
    m_reward = 35;
    m_dmg = 0.5f * ENEMY_DMG;
    m_speed = 2.4f * ENEMY_SPD;
    m_slowedSpeed = m_speed;
    m_slowRate = 1.f;
}

void HealerEnemy::setPos(GPVector2 vec)
{
    m_pos = vec;
}

GPTexture HealerEnemy::m_texture;

void HealerEnemy::setTexture(GPLib* gp)
{
    m_texture = gpTextureLoadFromFile(gp,
            "media/images/enemies_sprites/Enemy/Enemy 08-1.png");
}

void HealerEnemy::unloadTexture(GPLib* gp)
{
    gpTextureUnload(gp, m_texture);
}

void HealerEnemy::enemyDraw(GPLib* gp)
{
    GPRect rect = {(float)RENDER_SIZE * ((int)m_direction % MAX_LINE_E),
                    (float)RENDER_SIZE * ((int)m_direction / MAX_LINE_E),
                    RENDER_SIZE, RENDER_SIZE};
    GPRect dest = {m_pos.x, m_pos.y, 32.f, 32.f};
    gpDrawTextureEx2(gp, m_texture, rect, dest, GP_CWHITE);
    gpDrawCircle(gp, {m_pos.x + 16, m_pos.y + 16}, m_healRadius,
                {0.f, 1.f, 0.1f, 1.f});
    drawLifeBar(gp);
}

void HealerEnemy::enemyUpdate(GPLib* gp, std::vector<Enemy*>& enemies)
{
    if (m_currentCell < m_path.m_pathCells.size() - 1)
        enemyMove(gp);
    m_canHeal = false;
    m_healTimer += gpGetFrameTime(gp);

    if (m_healTimer >= m_healDelay)
    {
        m_healTimer = 0.f;
        for (auto e : enemies)
        {
            GPRect rect = {e->getPos().x, e->getPos().y,
                        RENDER_SIZE, RENDER_SIZE};
            if (utils::rectOverlapCircle({m_pos.x + 16, m_pos.y + 16},
                    m_healRadius, rect))
                {
                    m_canHeal = true;
                    e->heal(m_healAmount);
                }
        }
        if (!m_canHeal)
            heal(m_healAmount);
    }
}