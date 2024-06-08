#include "ui.h"
#include "cursor.h"
#include "game.h"
#include <iostream>

int g_towerCost[TOWER_NUMBER] = {BASIC_TURRET_COST,
                            SLOWING_TURRET_COST,
                            EXPLOSIVE_TURRET_COST};

GPTexture ButtonShop::m_shopTexture = GPTexture();

bool Button::isMouseOnButton(GPVector2 mouse)
{
    return utils::isMouseInsideRect({m_pos.x, m_pos.y, m_width, m_height}, mouse);
}

bool Button::buttonClicked(GPVector2 mouse, GPLib* gp)
{
    return (this->isMouseOnButton(mouse) &&
        gpMouseButtonIsPressed(gp, GP_MOUSE_BUTTON_1));
}

ButtonShop::ButtonShop(GPLib* gp)
{
    m_shopTexture = gpTextureLoadFromFile(gp,
            "media/images/towers_sprites/turrets_sprites0.png");
}

TowerSprite getSpriteFromType(TowerType type)
{
    switch(type)
    {
        case TowerType::E_BASIC:
            return TowerSprite::E_BASIC;
        case TowerType::E_EXPLOSIVE:
            return TowerSprite::E_EXPLOSIVE;
        case TowerType::E_SLOWING:
            return TowerSprite::E_SLOWING;
        default:
            return TowerSprite::E_BASIC;
    }
}

ButtonShop::ButtonShop(GPLib* gp, TowerType type, GPVector2 pos, int cost)
{
    m_pos = pos;
    m_width = RENDER_SIZE;
    m_height = RENDER_SIZE;
    m_towerType = type;
    m_isClicked = false;
    m_spriteIndex = getSpriteFromType(type);
    m_cost = cost;
}

void ButtonShop::drawButton(GPLib* gp, Cursor cur, GPFont font)
{
    GPRect rect = {(float)RENDER_SIZE * ((int)this->m_spriteIndex % MAX_LINE_T),
                         (float)RENDER_SIZE * ((int)this->m_spriteIndex / MAX_LINE_T),
                            RENDER_SIZE, RENDER_SIZE};
    GPRect dest = {m_pos.x, m_pos.y, 32.f, 32.f};
    gpDrawTextureEx2(gp, m_shopTexture, rect, dest, GP_CWHITE);
    gpDrawText(gp, font, {m_pos.x + 5, m_pos.y - 15}, GP_CBLACK, "%d", m_cost);
    if (m_isClicked)
        cur.drawCursor(gp, m_shopTexture, rect);
}

bool turretCanBePlaced(Game* game, GPRect rect)
{
    GPRect mapRect = {MAP_OFFSET + RENDER_SIZE, MAP_OFFSET + RENDER_SIZE,
                    (MAP_WIDTH - 2) * RENDER_SIZE, (MAP_HEIGHT - 2) * RENDER_SIZE};
    if (!utils::twoRectOverlap(mapRect, rect))
        return false;

    for (int i = 0; i < game->getTowerList().size(); ++i)
    {
        Tower* current = game->getTower(i);
        GPRect currentRect = {current->getPos().x, current->getPos().y, RENDER_SIZE, RENDER_SIZE};

        if (utils::twoRectOverlap(rect, currentRect))
            return false;
    }

    for (int i = 0; i < CELLS_NB; ++i)
    {
        Cell& cell = game->getMap().getCell(i);
        if (cell.m_type != CellType::E_FIELD)
        {
            GPRect cellRect = {cell.m_position.x, cell.m_position.y, 
                            RENDER_SIZE, RENDER_SIZE};
            if (utils::twoRectOverlap(rect, cellRect))
                return false;
        }
    }
    return true;
}

void ButtonShop::action(Cursor cur, Game* game)
{
    if(!m_isClicked)
        return;
    m_isClicked = false;
    Tower* tower;
    switch(m_towerType)
    {
        case TowerType::E_BASIC:
            tower = new BasicTower(cur.getPos());
            break;
        case TowerType::E_SLOWING:
            tower = new SlowingTower(cur.getPos());
            break;
        case TowerType::E_EXPLOSIVE:
            tower = new ExplosiveTower(cur.getPos());
            break;
        default:
            tower = new BasicTower(cur.getPos());
            break;
    }
    game->addCurrency(-m_cost);
    game->addTower(tower);
}

void ButtonShop::updateButton(GPLib* gp, Cursor& cur, Game* game)
{
    GPRect rect = {cur.getPos().x + 5, cur.getPos().y + 4, RENDER_SIZE - 5, RENDER_SIZE - 4};
    bool canBePlaced = turretCanBePlaced(game, rect);
    if (!canBePlaced)
        cur.color = {1.f, 0.f, 0.f, 0.75f};
    else
        cur.color = GP_CWHITE;
    if (this->buttonClicked(gpMousePosition(gp), gp) && game->getCurrency() >= m_cost)
        m_isClicked = true;
    else if (m_isClicked && gpMouseButtonIsPressed(gp, GP_MOUSE_BUTTON_1) && canBePlaced)
        action(cur, game);
    else if (m_isClicked && gpMouseButtonIsPressed(gp, GP_MOUSE_BUTTON_1))
        m_isClicked = false;
}

Interface::Interface(GPLib* gp, GPVector2 pos)
{
    m_pos = pos;
    for (int i = 0; i < TOWER_NUMBER; ++i)
    {
        m_buttons.push_back(new ButtonShop(gp, TowerType(i),
                    {pos.x + 32.f * i, pos.y + 32.f}, g_towerCost[i]));
    }
}

Interface::~Interface()
{
    /*for (auto b : m_buttons)
    {
        delete b;
    }*/
}

void Interface::drawInterface(GPLib* gp, Cursor cur, GPFont font)
{
    for (int i = 0; i < m_buttons.size(); ++i)
    {
        (*m_buttons[i]).drawButton(gp, cur, font);
    }
}

void Interface::updateInterface(GPLib* gp, Cursor& cur, Game* game)
{
    for (int i = 0; i < m_buttons.size(); ++i)
    {
        (*m_buttons[i]).updateButton(gp, cur, game);
    }
}

GPFont UI::m_font;

UI::UI(GPLib* gp)
{
    ButtonShop b(gp); // Instantiate static Texture
    m_font = gpFontLoad(gp, "media/fonts/chicken.ttf", 15.f);
    Interface in(gp, {928, 32});
    m_interfaces.push_back(in);
    m_pos = {0, 0};
}

void UI::unload(GPLib* gp)
{
    gpFontUnload(gp, m_font);
}

void UI::drawUI(GPLib* gp, Game& game)
{
    for (int i = 0; i < m_interfaces.size(); ++i)
    {
        m_interfaces[i].drawInterface(gp, m_cur, m_font);
    }
    game.drawLifeBar();
}

void UI::updateUI(GPLib* gp, Game* game)
{
    m_cur.updateCursor(gp);
    for (int i = 0; i < m_interfaces.size(); ++i)
    {
        m_interfaces[i].updateInterface(gp, m_cur, game);
    }
}