#include "tower.h"
#include "enemy.h"

BasicTower::BasicTower(GPVector2 pos)
{
    m_pos = pos;
    m_shootTimer = 0.f;
    m_spriteIndex = TowerSprite::E_BASIC;
    m_range = 150.f;
    m_fireRate = 0.45f;
    m_cost = 150;
    m_damage = 4;
}

void BasicTower::drawTower(GPLib* gp)
{
    Tower::drawTower(gp);
}

void BasicTower::updateTower(GPLib* gp, std::vector<Enemy*> enemies, Game& game)
{
    tryToUpgrade(gp, game);
    Tower::updateTower(gp, enemies, game);
}