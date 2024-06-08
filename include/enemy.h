#pragma once

#include <gp/gp.h>

#include "entity.h"
#include "map.h"
#include "utils.h"
#include "defs.h"

#define MAX_LINE_E 3

enum class EnemyType
{
    E_WEAK,
    E_STRONG,
    E_HEALER,

    E_COUNT
};

enum class EnemySprite
{
    E_DOWN = 1,
    E_LEFT = 4,
    E_RIGHT = 7,
    E_UP = 10
};

class Enemy : public Entity
{
protected:
    float               m_dmg;
    float               m_speed;
    float               m_slowedSpeed;
    int                 m_waveCost;
    int                 m_reward;
    EnemySprite         m_direction;
    int                 m_currentCell;
public:
    Enemy() {}

    static Path         m_path;
    float               m_slowRate;

    static void     setPath(Path& p);

    Path&           getPath() const { return m_path; }
    int             getReward() const { return m_reward; }
    int             getDmg() const { return m_dmg; }
    int             getLife() const { return m_currentLife; }

    void            takeDmg(int d);
    void            heal(int amount);



    void            drawLifeBar(GPLib* gp);

    virtual void    enemyUpdate(GPLib* gp, std::vector<Enemy*>& enemies) = 0;
    virtual void    enemyDraw(GPLib* gp) = 0;
    virtual void    setPos(GPVector2 vec) = 0;
    bool            enemyOnCastle();
    void            enemyMove(GPLib* gp);
};

class WeakEnemy : public Enemy // cost 1
{
private:
public:
    WeakEnemy();
    static GPTexture    m_texture;

    static void setTexture(GPLib* gp);
    static void unloadTexture(GPLib* gp);

    void        enemyDraw(GPLib* gp) override;
    void        enemyUpdate(GPLib* gp, std::vector<Enemy*>& enemies) override;
    void        setPos(GPVector2 vec) override;
};

class StrongEnemy : public Enemy // cost 3
{
public:
    StrongEnemy();
    static GPTexture    m_texture;

    static void setTexture(GPLib* gp);
    static void unloadTexture(GPLib* gp);

    void        enemyDraw(GPLib* gp) override;
    void        enemyUpdate(GPLib* gp, std::vector<Enemy*>& enemies) override;
    void        setPos(GPVector2 vec) override;
};

#define HEAL_DELAY 0.65f

class HealerEnemy : public Enemy // cost 6
{
private:
    float   m_healRadius;
    float   m_healAmount;
    float   m_healTimer;
    float   m_healDelay;
    bool    m_canHeal;
public:
    HealerEnemy();
    static GPTexture    m_texture;

    static void setTexture(GPLib* gp);
    static void unloadTexture(GPLib* gp);

    void        enemyDraw(GPLib* gp) override;
    void        enemyUpdate(GPLib* gp, std::vector<Enemy*>& enemies) override;
    void        setPos(GPVector2 vec) override;
};

Enemy* createEnemy(int a);