#include <string>

#include "path.h"
#include "map.h"
#include "utils.h"
#include "defs.h"


enum class Directions
{
    E_UP,
    E_LEFT,
    E_DOWN,
    E_RIGHT,

    E_COUNT
};

Directions getDirection(GPVector2 direction)
{
    GPVector2 cardinalPoints[] = { {0, -1}, {-1, 0}, {0, 1}, {1, 0} };
    if (direction.x == cardinalPoints[0].x
        && direction.y == cardinalPoints[0].y)
        return Directions::E_UP;
    if (direction.x == cardinalPoints[1].x
        && direction.y == cardinalPoints[1].y)
        return Directions::E_LEFT;
    if (direction.x == cardinalPoints[2].x
        && direction.y == cardinalPoints[2].y)
        return Directions::E_DOWN;
    if (direction.x == cardinalPoints[3].x
        && direction.y == cardinalPoints[3].y)
        return Directions::E_RIGHT;
    return Directions::E_DOWN;
/*    int angle = atan2f(direction.y, direction.x);

    if(angle >= PI/4 && angle < 3*PI/4)
        return Directions::E_DOWN;
    else if(angle >= 3*PI/4 && angle < 5*PI/4)
        return Directions::E_LEFT;
    else if(angle >= 5*PI/4 && angle < 7*PI/4)
        return Directions::E_UP;
    else if(angle >= 7*PI/4 && angle < PI/4)
        return Directions::E_RIGHT;
    return Directions::E_DOWN;*/
}

bool getNextCell(Cell* cells, int& x, int& y, GPVector2& direction)
{
    if (cells[x + y * MAP_WIDTH].m_type == CellType::E_END)
        return false;
    GPVector2 cardinalPoints[] = { {0, -1}, {-1, 0}, {0, 1}, {1, 0} };

    int currentDir = (int)getDirection(direction);

    int oppsiteDir = (currentDir + 2) % (int)Directions::E_COUNT;

    for (int i = 0; i < (int)Directions::E_COUNT; ++i)
    {
        if (currentDir != oppsiteDir)
        {
            int newX = x + cardinalPoints[currentDir].x;
            int newY = y + cardinalPoints[currentDir].y;

            if (cells[newX + newY * MAP_WIDTH].m_type == CellType::E_PATH
                || cells[newX + newY * MAP_WIDTH].m_type == CellType::E_END)
            {
                x = newX;
                y = newY;
                direction = cardinalPoints[currentDir];
                return true;
            }
        }

        currentDir = (currentDir + 1) % (int)Directions::E_COUNT;
    }
    return false;
}

GPVector2 getFirstDirection(Cell* cells, int x, int y)
{
    if (cells[(x - 1) + y * MAP_WIDTH].m_type == CellType::E_PATH)
        return {-1.f, 0.f};
    else if (cells[x + (y + 1) * MAP_WIDTH].m_type == CellType::E_PATH)
        return {0.f, 1.f};
    else if (cells[(x + 1) + y * MAP_WIDTH].m_type == CellType::E_PATH)
        return {1.f, 0.f};
    else if (cells[x + (y - 1) * MAP_WIDTH].m_type == CellType::E_PATH)
        return {0.f, -1.f};
    return {0.f, 1.f};
}

void Path::setupPath(Cell* cells)
{
    Cell currentCell;
    int i = 0;
    int j = 0;
    for (int y = 0; y < MAP_HEIGHT; ++y)
    {
        for (int x = 0; x < MAP_WIDTH; ++x)
        {
            if (cells[x + y * MAP_WIDTH].m_type == CellType::E_START)
            {
                 currentCell = cells[x + y * MAP_WIDTH];
                 i = x;
                 j = y;
                 break;
            }
        }
    }

    m_pathCells.push_back(currentCell);
    GPVector2 direction = getFirstDirection(cells, i, j);
    direction = utils::normalize(direction);
    int nextX = i;
    int nextY = j;

    while (getNextCell(cells, nextX, nextY, direction))
    {
        currentCell = cells[nextX + nextY * MAP_WIDTH];
        m_pathCells.push_back(currentCell);
    }
}

void Path::drawPath(GPLib* gp)
{/*
    GPFont font = gpFontLoad(gp, "media/fonts/chicken.ttf", 20);
    for (int i = 0; i < m_pathCells.size(); ++i)
    {
        gpDrawText(gp, font, m_pathCells[i].m_position, {1.f, 0.f, 0.f, 1.f},
            std::to_string(i).c_str());
    }*/
}