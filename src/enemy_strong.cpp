#include "enemy.h"
#include <string>


StrongEnemy::StrongEnemy()
{
    m_maxLife = 50;
    m_currentLife = m_maxLife;
    m_direction = EnemySprite::E_DOWN;
    m_currentCell = 0;
    m_waveCost = 3;
    m_reward = 20;
    m_dmg = 3 * ENEMY_DMG;
    m_speed = 1.2f * ENEMY_SPD;
    m_slowedSpeed = m_speed;
    m_slowRate = 1.f;
}

void StrongEnemy::setPos(GPVector2 vec)
{
    m_pos = vec;
}

GPTexture StrongEnemy::m_texture;

void StrongEnemy::setTexture(GPLib* gp)
{
    m_texture = gpTextureLoadFromFile(gp,
            "media/images/enemies_sprites/Enemy/Enemy 05-1.png");
}

void StrongEnemy::unloadTexture(GPLib* gp)
{
    gpTextureUnload(gp, m_texture);
}

void StrongEnemy::enemyDraw(GPLib* gp)
{
    GPRect rect = {(float)RENDER_SIZE * ((int)m_direction % MAX_LINE_E),
                    (float)RENDER_SIZE * ((int)m_direction / MAX_LINE_E),
                    RENDER_SIZE, RENDER_SIZE};
    GPRect dest = {m_pos.x, m_pos.y, 32.f, 32.f};
    gpDrawTextureEx2(gp, m_texture, rect, dest, GP_CWHITE);
    drawLifeBar(gp);
}

void StrongEnemy::enemyUpdate(GPLib* gp, std::vector<Enemy*>& enemies)
{
    if (m_currentCell < m_path.m_pathCells.size() - 1)
        enemyMove(gp);
}