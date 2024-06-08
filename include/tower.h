#pragma once

#include <vector>

#include <gp/gp.h>
#include "entity.h"
#include "defs.h"
#include "utils.h"
#include "enemy.h"
#include "projectile.h"


#define MAX_LINE_T 4



enum class TowerSprite
{
    E_BASIC = 12,
    E_SLOWING = 9,
    E_EXPLOSIVE = 5
};
enum class TowerType
{
    E_BASIC,
    E_SLOWING,
    E_EXPLOSIVE
};

#define LEVEL_MAX 5
class Tower : public Object
{
protected:
    int                     m_level = 1;
    float                   m_range;
    float                   m_fireRate;
    int                     m_cost;
    float                   m_angle;
    float                   m_shootTimer;
    int                     m_damage;
    Enemy*                  m_target;
    static GPFont           m_font;
    static GPTexture        m_texture;
    TowerSprite             m_spriteIndex;
    std::vector<Projectile> m_projectiles;
public:
    Tower();
    virtual void    drawTower(GPLib* gp);
    virtual void    updateTower(GPLib* gp, std::vector<Enemy*> enemies, Game& game);
    void            setTarget(Enemy* enemy);
    static void     setTexture(GPLib* gp);
    static void     unloadTexture(GPLib* gp);

    void            shoot();
    Enemy*          getTarget() { return m_target; }
    void            tryToUpgrade(GPLib* gp, Game& game);
    int             getCost() const { return this->m_cost; }
    int             getDamage() const { return this->m_damage; }
    int             getFireRate() const { return this->m_fireRate; }
    int             getRange() const { return this->m_range; }
};
// GPVector2 toTarget = target - pos
// float rotate = atan2f(toTarget.y, toTarget.x)
class BasicTower : public Tower
{
public:
    BasicTower(GPVector2 pos);
    void    drawTower(GPLib* gp) override;
    void    updateTower(GPLib* gp,  std::vector<Enemy*> enemies, Game& game) override;
};

#define SLOWING_ATK_DELAY 1.35f

class SlowingTower : public Tower
{
private:
    float m_slowRate;
public:
    SlowingTower(GPVector2 pos);
    void    drawTower(GPLib* gp) override;
    void    updateTower(GPLib* gp,  std::vector<Enemy*> enemies, Game& game) override;
};

class ExplosiveTower : public Tower
{
private:
    float m_attackRadius;
public:
    ExplosiveTower(GPVector2 pos);
    void    drawTower(GPLib* gp) override;
    void    updateTower(GPLib* gp,  std::vector<Enemy*> enemies, Game& game) override;
};

std::vector<Tower*>  initTowers(GPLib* gp);