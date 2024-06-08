#pragma once

#include "enemy.h"
#include <vector>

#define PRELAUNCH_TIMER 2.f
#define LAUNCH_TIMER 15.f

enum class WaveState
{
    E_PRELAUNCH,
    E_ONGOING
};

class Game;

class Wave
{
private:
    int                 m_currentWave;
    float               m_prelaunchTimer;
    float               m_launchTimer;
    float               m_waitingTime;
    std::vector<Enemy*> m_waitingEnemies;
    std::vector<Enemy*> m_enemies;
    WaveState           m_state;
public:
    Wave();
    ~Wave();
    void                setEnemies();
    std::vector<Enemy*> getEnemies() const { return m_enemies; }
    int                 getCurrentWave() { return m_currentWave; }

    void                prelaunchUpdate(GPLib* gp);
    void                ongoingUpdate(GPLib* gp, Game& game);

    void                waveUpdate(GPLib* gp, Game& game);
    void                waveDraw(GPLib* gp);

    void                operator++(int);
};