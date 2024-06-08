#include "enemy.h"
#include <string>


WeakEnemy::WeakEnemy()
{
    m_maxLife = 20;
    m_currentLife = m_maxLife;
    m_direction = EnemySprite::E_DOWN;
    m_currentCell = 0;
    m_waveCost = 1;
    m_reward = 5;
    m_dmg = ENEMY_DMG;
    m_speed = 2.f * ENEMY_SPD;
    m_slowedSpeed = m_speed;
    m_slowRate = 1.f;
}

void WeakEnemy::setPos(GPVector2 vec)
{
    m_pos = vec;
}

GPTexture WeakEnemy::m_texture;

void WeakEnemy::setTexture(GPLib* gp)
{
    m_texture = gpTextureLoadFromFile(gp,
            "media/images/enemies_sprites/Enemy/Enemy 06-1.png");
}

void WeakEnemy::unloadTexture(GPLib* gp)
{
    gpTextureUnload(gp, m_texture);
}

void WeakEnemy::enemyDraw(GPLib* gp)
{
    GPRect rect = {(float)RENDER_SIZE * ((int)m_direction % MAX_LINE_E),
                    (float)RENDER_SIZE * ((int)m_direction / MAX_LINE_E),
                    RENDER_SIZE, RENDER_SIZE};
    GPRect dest = {m_pos.x, m_pos.y, 32.f, 32.f};
    gpDrawTextureEx2(gp, m_texture, rect, dest, GP_CWHITE);
    drawLifeBar(gp);
    /*GPFont font = gpFontLoad(gp, "media/fonts/chicken.ttf", 15);
    gpDrawText(gp, font, m_pos, {0.f, 1.f, 0.f, 1.f},
            std::to_string(m_currentCell+1).c_str());*/
}

void WeakEnemy::enemyUpdate(GPLib* gp, std::vector<Enemy*>& enemies)
{
    if (m_currentCell < m_path.m_pathCells.size() - 1)
        enemyMove(gp);
}