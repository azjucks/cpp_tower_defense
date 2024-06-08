#pragma once

#include <gp/gp.h>
#include <vector>
#include "defs.h"
#include "path.h"

#define MAX_LINE 5

/*char g_shop[30] =
{
    '#', '#', '#',
    '#', '#', '#',
    '#', '#', '#',
    '#', '#', '#',
    '#', '#', '#',
    '#', '#', '#',
    '#', '#', '#',
    '#', '#', '#',
    '#', '#', '#',
    '#', '#', '#'
};*/

enum class CellType
{
    E_START,
    E_PATH,
    E_FIELD,
    E_WATER,
    E_END,

    E_NONE
};


enum class CellSprite
{
    E_PATH_SPRITE = 3,

    E_FIELD_SPRITE = 6,

    E_R_SPRITE = 7,
    E_L_SPRITE = 5,
    E_U_SPRITE = 1,
    E_D_SPRITE = 11,

    E_UR_SPRITE = 2,
    E_UL_SPRITE = 0,
    E_DR_SPRITE = 12,
    E_DL_SPRITE = 10,

    E_WATER = 8
};

struct Cell
{
    CellType    m_type;
    GPVector2   m_position;
    CellSprite  m_spriteIndex;
    bool        m_isBorder; //useful bool only on certain cases
};

enum class Environment
{
    E_GRASSLAND,
    E_DESERT,
    E_TUNDRA,

    E_NONE
};

class Game;

class Map
{
private:
    GPTexture   m_texture;
    Path        m_path;
    Environment m_env;
    Cell        m_cells[CELLS_NB];
public:
    Map() {};
    void        setupEnvironment(const char* str, GPLib* gp);
    Environment getEnvironment() const;
    void        setMapSprites();
    void        setPath(Path& p);
    void        setCell(int index, Cell cell);
    void        drawMap(GPLib* gp);
    Path&       getPath() { return m_path; }
    Cell*       getCells() { return m_cells; }
    Cell&       getCell(int x, int y) { return m_cells[x + y * MAP_WIDTH]; }
    Cell&       getCell(int i) { return m_cells[i]; }
};

std::vector<Map>    initMaps(GPLib* gp);