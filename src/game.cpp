#include "game.h"

#include <string>

Game::Game(GPLib* gp)
{
    m_gp = gp;
    m_score = 0;
    m_time = gpGetTime(gp);
    m_timer = 0.f;
    m_currency = 150;
    m_currentLife = CASTLE_LIFE;
    m_maxLife = m_currentLife;
    m_font = gpFontLoad(gp, "media/fonts/chicken.ttf", 50.f);
    m_gameSpd = 1;
    m_maps = initMaps(gp);
    m_towers = initTowers(gp);
    m_currentMapIndex = 0;
    m_wave = Wave();
    m_UI = UI(gp);
}

void Game::gameUnload()
{
    WeakEnemy::unloadTexture(m_gp);
    StrongEnemy::unloadTexture(m_gp);
    HealerEnemy::unloadTexture(m_gp);
    Tower::unloadTexture(m_gp);
    UI::unload(m_gp);
    gpFontUnload(m_gp, m_font);
}

Game::~Game()
{
    for(int i = 0; i < m_towers.size(); ++i)
    {
        delete m_towers[i];
    }
}

void Game::gameInit()
{
    WeakEnemy::setTexture(m_gp);
    StrongEnemy::setTexture(m_gp);
    HealerEnemy::setTexture(m_gp);
    Tower::setTexture(m_gp);
}

void Game::addTower(Tower* tower)
{
    m_towers.push_back(tower);
}

void Game::addCurrency(int a)
{
    m_currency += a;
}

void Game::takeDmg(int d)
{
    m_currentLife = m_currentLife - d > 0 ? m_currentLife - d : 0;
}

void Game::drawLifeBar()
{
    gpDrawRectFilled(m_gp, {0, WINDOW_HEIGHT - 20, WINDOW_WIDTH, 20},
                    {1.f, 0.521f, 0.521f, 1.f});
    gpDrawRectFilled(m_gp, {0, WINDOW_HEIGHT - 20, WINDOW_WIDTH * (m_currentLife/m_maxLife), 20},
                    {0.835f, 0.043f, 0.043f, 1.f});
}

void Game::drawInfos()
{
    gpDrawText(m_gp, m_font,
            {0, 50},
            GP_CBLACK,
            "Currency: %d   Wave: %d", m_currency, m_wave.getCurrentWave());
}

void Game::gameDraw()
{
    m_maps[0].drawMap(m_gp);
    for (int i = 0; i < m_towers.size(); ++i)
        (*m_towers[i]).drawTower(m_gp);
    m_UI.drawUI(m_gp, *this);
    m_wave.waveDraw(m_gp);
    drawInfos();
    if (m_currentLife <= 0)
    {
        drawEnd();
    }
}

void Game::gameUpdate()
{
    for (int i = 0; i < m_gameSpd; ++i)
    {
        m_UI.updateUI(m_gp, this);
        m_wave.waveUpdate(m_gp, *this);
        for (auto t : m_towers)
        {
            t->updateTower(m_gp, m_wave.getEnemies(), *this);
        }
    }
}

void Game::drawEnd()
{
    gpDrawTextAligned(m_gp, m_font,
            {WINDOW_WIDTH/2, WINDOW_HEIGHT/2},
            GP_CBLACK, GP_TEXTALIGN_CENTER,
            "Game Over");
    gpDrawTextAligned(m_gp, m_font,
            {WINDOW_WIDTH/2, WINDOW_HEIGHT/2 + 50},
            GP_CBLACK, GP_TEXTALIGN_CENTER, "Score: %d", m_wave.getCurrentWave());
}

void Game::gameRun()
{
    std::cout << std::endl; // Force flush stdout

    gameInit();
    Enemy::setPath(m_maps[0].getPath());

    while (gpWindowShouldClose(m_gp) == false)
    {
            // Poll events (mouse, keyboards, etc...)
            gpEventsPoll(m_gp);

            // TODO: Update game
            if (m_currentLife > 0)
                gameUpdate();
            else
                m_timer += gpGetFrameTime(m_gp);
            
            if (m_timer >= END_TIMER)
                return;

            if (gpKeyIsPressed(m_gp, GP_KEY_RIGHT))
                m_gameSpd += m_gameSpd < MAX_GAME_SPD ? 2 : 0;
            if (gpKeyIsPressed(m_gp, GP_KEY_LEFT))
                m_gameSpd -= m_gameSpd == MIN_GAME_SPD ? 0 : 2;

            gpDrawBegin(m_gp);
            gpDrawClearBackground(m_gp, {0.68f, 0.85f, 0.90f, 1.f});

            gameDraw();
            
            gpDrawEnd(m_gp);
    }
}