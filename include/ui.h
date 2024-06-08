#pragma once

#include <gp/gp.h>
#include <vector>

#include "object.h"
#include "utils.h"
#include "tower.h"
#include "cursor.h"

class Game;

class Button : public Object
{
protected:
    bool        m_isClicked;
    GPTexture   m_texture;
public:
    Button() {}
    ~Button() {}
    bool            isMouseOnButton(GPVector2 mouse);
    bool            buttonClicked(GPVector2 mouse, GPLib* gp);
    virtual void    drawButton(GPLib* gp, Cursor cur, GPFont font) = 0;
    virtual void    updateButton(GPLib* gp, Cursor& cur, Game* game) = 0;
};

class ButtonShop : public Button
{
protected:
    static GPTexture    m_shopTexture;
private:
    TowerType           m_towerType;
    TowerSprite         m_spriteIndex;
    int                 m_cost;
public:
    ~ButtonShop() {}
    ButtonShop(GPLib* gp);
    ButtonShop(GPLib* gp, TowerType type, GPVector2 pos, int cost);
    void action(Cursor cur, Game* game);
    void drawButton(GPLib* gp, Cursor cur, GPFont font) override;
    void updateButton(GPLib* gp, Cursor& cur, Game* game) override;
};

class Interface : public Object
{
protected:
    std::vector<Button*>    m_buttons;
    GPRect                  m_rect;
public:
    Interface(GPLib* gp, GPVector2 pos);
    ~Interface();
    void drawInterface(GPLib* gp, Cursor cur, GPFont font);
    void updateInterface(GPLib* gp, Cursor& cur, Game* game);
};

class UI : public Object
{
private:
    std::vector<Interface>  m_interfaces;
    Cursor                  m_cur;
    static GPFont           m_font;
public:
    UI() {}
    ~UI() {}
    UI(GPLib* gp);
    static void unload(GPLib* gp);
    void drawUI(GPLib* gp, Game& game);
    void updateUI(GPLib* gp, Game* game);
};