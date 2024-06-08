#include "wave.h"
#include "game.h"

#include <iostream>
#include <string>
#include <cmath>

void Wave::setEnemies()
{
    for (int i = 0; i < m_currentWave*2;)
    {
        Enemy* e;
        if (i % 2 == 0)
        {
            e = new WeakEnemy();
            i += 1;
        }
        else if (i % 3 == 0)
        {
            e = new HealerEnemy();
            i += 6;
        }
        else
        {
            e = new StrongEnemy();
            i += 3;
        }
        e->setPos(e->getPath().m_pathCells[0].m_position);
        m_waitingEnemies.push_back(e);
    }
    
    // int maxCost = log10(m_currentWave * 15) * (m_currentWave + 10 % m_currentWave); // log2 non ?
    // int i = 0;
    // while (i < maxCost)
    // {
    //     Enemy* enemy;
    //     int enemyCost = 0;

    //     while (enemyCost == 0)
    //     {
    //         EnemyType type = (EnemyType)(rand(0, EnemyType::E_COST);
    //         enemy = 

    //         if (enemyCost + i > maxCost)
    //             enemyCost = 0;
    //     }
    //     m_enemies.push_back(createEnemy(a));
    // }
}

Wave::~Wave()
{
    for (int i = 0; i < m_enemies.size(); ++i)
    {
        delete m_enemies[i];
    }

    for (int i = 0; i < m_waitingEnemies.size(); ++i)
    {
        delete m_waitingEnemies[i];
    }
}

Wave::Wave()
{
    m_currentWave = 1;
    m_prelaunchTimer = PRELAUNCH_TIMER;
    m_launchTimer = LAUNCH_TIMER;
    m_state = WaveState::E_PRELAUNCH;
}

void Wave::prelaunchUpdate(GPLib* gp)
{
    if (m_waitingEnemies.size() == 0)
    {
        setEnemies();
        m_waitingTime = LAUNCH_TIMER / m_waitingEnemies.size();
    }
    m_prelaunchTimer -= gpGetFrameTime(gp);
    if (m_prelaunchTimer <= 0)
    {
        m_state = WaveState::E_ONGOING;
        m_prelaunchTimer = PRELAUNCH_TIMER;
    }
}

void Wave::ongoingUpdate(GPLib* gp, Game& game)
{
    if (m_launchTimer >= 0 || m_waitingEnemies.size() != 0)
    {
        if (m_launchTimer <= m_waitingTime * m_waitingEnemies.size())
        {
            m_enemies.push_back(m_waitingEnemies.back());
            m_waitingEnemies.pop_back();
        }
        m_launchTimer -= gpGetFrameTime(gp);
    }

    for (int i = 0; i < m_enemies.size(); ++i)
    {
        if (m_enemies[i]->getLife() <= 0)
        {
            game.addCurrency(m_enemies[i]->getReward());
            delete m_enemies[i];
            m_enemies[i] = m_enemies[m_enemies.size() - 1];
            m_enemies.pop_back();
            continue;
        }
        m_enemies[i]->enemyUpdate(gp, m_enemies);
        
        if (m_enemies[i]->enemyOnCastle())
        {
            game.takeDmg(m_enemies[i]->getDmg());
            std::cout << std::to_string(game.getLife()) << std::endl;
            delete m_enemies[i];
            m_enemies[i] = m_enemies[m_enemies.size() - 1];
            m_enemies.pop_back();
        }
    }

    if (m_enemies.size() == 0
        && m_waitingEnemies.size() == 0)
    {
        m_state = WaveState::E_PRELAUNCH;
        m_currentWave += 1;
        m_launchTimer = LAUNCH_TIMER;
        game.addCurrency(75);
    }
}

void Wave::waveUpdate(GPLib* gp, Game& game)
{
    if (m_state == WaveState::E_ONGOING)
    {
        ongoingUpdate(gp, game);
    }
    else
    {
        prelaunchUpdate(gp);
    }
    
}

void Wave::waveDraw(GPLib* gp)
{
    for (int i = 0; i < m_enemies.size(); ++i)
    {
        m_enemies[i]->enemyDraw(gp);
    }
}

void Wave::operator++(int)
{
    m_currentWave++;
}