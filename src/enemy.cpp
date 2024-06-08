#include "enemy.h"

#include <iostream>

Path Enemy::m_path;

void Enemy::setPath(Path& p)
{
    m_path = p;
}

Enemy* createEnemy(int availableCost)
{
    return nullptr;
    /*switch(availableCost)
    {
        
    }*/
}

void Enemy::heal(int amount)
{
    m_currentLife = m_currentLife + amount < m_maxLife ?
                    m_currentLife + amount : m_maxLife;
}

void Enemy::takeDmg(int d)
{
    m_currentLife = m_currentLife - d > 0 ? m_currentLife - d : 0;
}

bool Enemy::enemyOnCastle()
{
    if (m_currentCell == m_path.m_pathCells.size() - 1)
        return true;
    return false;
}

void Enemy::enemyMove(GPLib* gp)
{
    m_speed = m_slowedSpeed * m_slowRate;
    Cell& currentCell = m_path.m_pathCells[m_currentCell];
    Cell& targetCell = m_path.m_pathCells[m_currentCell + 1];
    GPVector2 dest = targetCell.m_position;
    float delta = gpGetFrameTime(gp);
    if (utils::vector2Compare(m_pos, dest, m_speed/5))
    {
        setPos(dest);
        m_currentCell += 1;
    }
    else if (currentCell.m_position.x > dest.x
            && m_pos.x > dest.x)
    {
        m_direction = EnemySprite::E_LEFT;
        m_pos.x -= m_speed * delta;
    }
    else if (currentCell.m_position.y > dest.y
            && m_pos.y > dest.y)
    {
        m_direction = EnemySprite::E_UP;
        m_pos.y -= m_speed * delta;
    }
    else if (currentCell.m_position.x < dest.x
            && m_pos.x < dest.x)
    {
        m_direction = EnemySprite::E_RIGHT;
        m_pos.x += m_speed * delta;
    }
    else if (currentCell.m_position.y < dest.y
            && m_pos.y < dest.y)
    {
        m_direction = EnemySprite::E_DOWN;
        m_pos.y += m_speed * delta;
    }
}

void Enemy::drawLifeBar(GPLib* gp)
{
    gpDrawRectFilled(gp, {m_pos.x, m_pos.y - 5, 32.f, 5},
                    {1.f, 0.521f, 0.521f, 0.8f});
    gpDrawRectFilled(gp, {m_pos.x, m_pos.y - 5, 32.f * (m_currentLife/m_maxLife), 5},
                    {0.835f, 0.043f, 0.043f, 1.f});
}