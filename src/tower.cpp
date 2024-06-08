#include "tower.h"
#include "enemy.h"
#include "game.h"

#include <iostream>

GPTexture Tower::m_texture;

std::vector<Tower*>  initTowers(GPLib* gp)
{
    std::vector<Tower*> towers;

    return towers;
}

Tower::Tower()
{
    m_target = nullptr;
}

GPFont Tower::m_font;

void Tower::setTexture(GPLib* gp)
{
    m_font = gpFontLoad(gp, "media/fonts/chicken.ttf", 15.f);
    m_texture = gpTextureLoadFromFile(gp,
            "media/images/towers_sprites/turrets_sprites0.png");
}

void Tower::unloadTexture(GPLib* gp)
{
    gpFontUnload(gp, m_font);
    gpTextureUnload(gp, m_texture);
}


void Tower::setTarget(Enemy* enemy)
{
    m_target = enemy;
}

void Tower::drawTower(GPLib* gp)
{
    GPRect rect = {(float)RENDER_SIZE * ((int)this->m_spriteIndex % MAX_LINE_T),
                         (float)RENDER_SIZE * ((int)this->m_spriteIndex / MAX_LINE_T),
                            RENDER_SIZE, RENDER_SIZE};

    if (m_target != nullptr)
    {
        GPVector2 targetVec = {m_target->getPos().x + 16, m_target->getPos().y + 16};
        m_angle = utils::getAngle(targetVec, {m_pos.x + 16, m_pos.y + 16}) + PI * 0.5;
    }
    gpDrawText(gp, m_font, {m_pos.x + 5, m_pos.y - 5}, GP_CBLACK, "Level: %d", m_level);

    if (m_level < LEVEL_MAX);
    {
        gpDrawText(gp, m_font, {m_pos.x - 15, m_pos.y + 37}, GP_CBLACK, "Click to upgrade");
        gpDrawText(gp, m_font, {m_pos.x - 15, m_pos.y + 47}, GP_CBLACK, "Upgrade cost: %d", (int)((float)m_cost / 2));
    }
    
    gpDrawTextureEx(gp, m_texture, rect, {m_pos.x + 16, m_pos.y + 16},
                    m_angle,
                    {1.f, 1.f}, nullptr, GP_CWHITE);
    for (auto p : m_projectiles)
    {
        p.projectileDraw(gp);
    }
    gpDrawCircleFilled(gp, {m_pos.x + 16, m_pos.y + 16}, m_range, {0.f, 0.f, 1.f, 0.05f});
}

void Tower::shoot()
{
    GPVector2 pos = {m_pos.x + 16, m_pos.y + 16};
    Projectile p(pos, m_target, m_damage);
    m_projectiles.push_back(p);
}

void Tower::tryToUpgrade(GPLib* gp, Game& game)
{
    GPRect rect = {m_pos.x, m_pos.y, RENDER_SIZE, RENDER_SIZE};

    if (utils::pointInsideRect(gpMousePosition(gp), rect)
        && gpMouseButtonIsPressed(gp, GP_MOUSE_BUTTON_1)
        && game.getCurrency() >= (int)((float)m_cost / 2)
        && m_level < LEVEL_MAX)
    {
        m_damage *= 1.2;
        m_range *= 1.05;
        m_level += 1;
        game.addCurrency(-m_cost/2);
    }
}

void Tower::updateTower(GPLib* gp, std::vector<Enemy*> enemies, Game& game)
{
    GPRect rect = {};
    for (int i = 0; i < m_projectiles.size(); ++i)
    {
        if (!m_projectiles[i].hasTarget())
        {
            m_projectiles[i] = m_projectiles[m_projectiles.size() - 1];
            m_projectiles.pop_back();
            continue;
        }
        m_projectiles[i].projectileUpdate(gp);

        if (m_projectiles[i].getTarget()->getLife() <= 0)
        {
            m_target = nullptr;
            m_projectiles[i].resetTarget();
        }

        if (m_projectiles[i].collideTarget())
        {
            m_projectiles[i] = m_projectiles[m_projectiles.size() - 1];
            m_projectiles.pop_back();
        }
    }
    if (m_target != nullptr)
    {
        rect = {m_target->getPos().x, m_target->getPos().y,
                            RENDER_SIZE, RENDER_SIZE};
        if (m_shootTimer >= m_fireRate)
        {
            shoot();
            m_shootTimer = 0.f;
        }
        m_shootTimer += gpGetFrameTime(gp);
    }
    if (m_target == nullptr || !utils::rectOverlapCircle(m_pos, m_range, rect))
    {
        for (auto e : enemies)
        {
            rect = {e->getPos().x, e->getPos().y,
                        RENDER_SIZE, RENDER_SIZE};
            if (utils::rectOverlapCircle(m_pos, m_range, rect))
            {
                m_target = e;
                return;
            }
        }
        m_target = nullptr;
    }
}