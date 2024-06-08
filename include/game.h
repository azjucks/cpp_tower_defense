#pragma once

#include <iostream>
#include <chrono>
#include <vector>

#include <gp/gp.h>
#include "tower.h"
#include "map.h"
#include "wave.h"
#include "ui.h"

#define MAX_GAME_SPD 16
#define MIN_GAME_SPD 1

#define END_TIMER 5.f

class Game
{
private:
    GPLib*              m_gp;
    int                 m_gameSpd;
    int                 m_score;
    float               m_currentLife;
    float               m_maxLife;
    float               m_timer;
    float               m_time;
    int                 m_currency;
    std::vector<Map>    m_maps;
    int                 m_currentMapIndex;
    Wave                m_wave;
    std::vector<Tower*> m_towers;
    UI                  m_UI;
public:
    Game(GPLib* gp);
    ~Game();
    GPFont              m_font;
    void                gameInit();
    void                gameUnload();
    void                drawLifeBar();
    void                drawEnd();
    void                drawInfos();
    void                gameDraw();
    void                gameUpdate();
    void                gameRun();
    void                loadSounds();
    void                addTower(Tower* tower);
    void                addCurrency(int a);
    void                takeDmg(int d);
    int                 getCurrency() const { return m_currency; }
    int                 getLife() const { return m_currentLife; }
    GPLib*              getGP() const { return m_gp; }
    Map                 getMap() const { return m_maps[m_currentMapIndex]; }
    Tower*              getTower(int i) const { return m_towers[i]; };
    std::vector<Tower*> getTowerList() const { return m_towers; }
};