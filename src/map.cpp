#include "map.h"
#include "defs.h"
#include "game.h"

#include <filesystem>
#include <string>
#include <iostream>
#include <fstream>


void Map::setupEnvironment(const char* str, GPLib* gp)
{
    std::string path = "media/images/field_sprites/";
    if (strcmp(str, "GRASSLAND") == 0)
    {
        this->m_env = Environment::E_GRASSLAND;
        path += "grassland.png";
        this->m_texture = gpTextureLoadFromFile(gp, path.c_str());
    }
    else if (strcmp(str, "DESERT") == 0)
    {
        this->m_env = Environment::E_DESERT;
        path += "desert.png";
        this->m_texture = gpTextureLoadFromFile(gp, path.c_str());
    }
    else if (strcmp(str, "TUNDRA") == 0)
    {
        this->m_env = Environment::E_TUNDRA;
        path += "tundra.png";
        this->m_texture = gpTextureLoadFromFile(gp, path.c_str());
    }
    else
        this->m_env = Environment::E_NONE;
}

Environment Map::getEnvironment() const
{
    return this->m_env;
}

CellType getCellType(char c)
{
    switch(c)
    {
        case 'S':
            return CellType::E_START;
        case 'P':
            return CellType::E_PATH;
        case '#':
            return CellType::E_FIELD;
        case 'W':
            return CellType::E_WATER;
        case 'E':
            return CellType::E_END;
        default:
            return CellType::E_NONE;
    }
}

void Map::setCell(int index, Cell cell)
{
    this->m_cells[index] = cell;
} 

unsigned int matrix_filter(Map& m, int x, int y)
{
    unsigned int res = 0;
    if (x != (MAP_WIDTH - 1) && m.getCell(x + 1, y).m_isBorder)
        res += 1;
    res <<= 1;
    if (y != 0 && m.getCell(x, y - 1).m_isBorder)
        res += 1;
    res <<= 1;
    if (x != 0 && m.getCell(x - 1, y).m_isBorder)
        res += 1;
    res <<= 1;
    if (y != (MAP_HEIGHT - 1) && m.getCell(x, y + 1).m_isBorder)
        res += 1;

    return res;
}

CellSprite getCellSprite(Map m, int x, int y)
{
    unsigned int index = matrix_filter(m, x, y);

    switch(index)
    {
        case 9:
            return CellSprite::E_UL_SPRITE;
        case 11:
            return CellSprite::E_U_SPRITE;
        case 12:
            return CellSprite::E_DL_SPRITE;
        case 3:
            return CellSprite::E_UR_SPRITE;
        case 6:
            return CellSprite::E_DR_SPRITE;
        case 14:
            return CellSprite::E_D_SPRITE;
        case 13:
            return CellSprite::E_L_SPRITE;
        case 7:
            return CellSprite::E_R_SPRITE;
        default:
            return CellSprite::E_FIELD_SPRITE;
    }
}

void Map::setMapSprites()
{
    for (int j = 0; j < MAP_HEIGHT; ++j)
    {
        for (int i = 0; i < MAP_WIDTH; ++i)
        {
            Cell& cell = this->m_cells[i + j * MAP_WIDTH];
                
            if (cell.m_type == CellType::E_FIELD)
                cell.m_spriteIndex = getCellSprite(*this, i, j);

            else if (cell.m_type == CellType::E_WATER)
                cell.m_spriteIndex = CellSprite::E_WATER;

            else if (cell.m_type == CellType::E_PATH
                    || cell.m_type == CellType::E_START
                    || cell.m_type == CellType::E_END)

                cell.m_spriteIndex = CellSprite::E_PATH_SPRITE;
        }
    }
}

void Map::setPath(Path& p)
{
    m_path = p;
}

std::vector<Map> initMaps(GPLib* gp)
{
    std::vector<Map>    maps;
    std::string         path = "media/maps/";
    for (const auto& entry : std::filesystem::directory_iterator(path))
    {
        Map                 m;
        std::ifstream       mapFile(entry.path());
        std::string         str;

        getline(mapFile, str);
        m.setupEnvironment(str.c_str(), gp);

        getline(mapFile, str);

        for (int j = 0; j < MAP_HEIGHT; ++j)
        {
            for (int i = 0; i < MAP_WIDTH; ++i)
            {
                char c = str[i];
                Cell cell;
                cell.m_type = getCellType(c);
                cell.m_position = {(float)i * RENDER_SIZE + MAP_OFFSET,
                                (float)j * RENDER_SIZE + MAP_OFFSET};

                if (cell.m_type != CellType::E_NONE)   
                    if (i == 0 || j == 0 || i >= (MAP_WIDTH - 1)
                    || j >= (MAP_HEIGHT - 1))

                    cell.m_isBorder = true;

                m.setCell(i + j * MAP_WIDTH, cell);
            }
            getline(mapFile, str);
        }
        m.setMapSprites();
        Path p;
        p.setupPath(m.getCells());
        m.setPath(p);
        maps.push_back(m);
        mapFile.close();
    }
    return maps;
}

void Map::drawMap(GPLib* gp)
{
    for (int j = 0; j < MAP_HEIGHT; ++j)
    {
        for (int i = 0; i < MAP_WIDTH; ++i)
        {
            Cell& cell = this->m_cells[i + j * MAP_WIDTH];
            if (cell.m_type == CellType::E_NONE)
                continue;
            GPRect rect = {(float)RENDER_SIZE * ((int)cell.m_spriteIndex % MAX_LINE),
                         (float)RENDER_SIZE * ((int)cell.m_spriteIndex / MAX_LINE),
                            RENDER_SIZE, RENDER_SIZE};
            GPRect dest = {MAP_OFFSET + 32.f * i, MAP_OFFSET + 32.f * j, 32.f, 32.f};
            gpDrawTextureEx2(gp, m_texture, rect, dest, GP_CWHITE);
        }
    }
    m_path.drawPath(gp);
}