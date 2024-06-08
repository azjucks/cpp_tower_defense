#pragma once

#include <vector>

#include <gp/gp.h>

class Cell;

class Path
{
public:
    std::vector<Cell> m_pathCells;

    Path() {}

    void    setupPath(Cell* cells);
    void    drawPath(GPLib* gp);
};