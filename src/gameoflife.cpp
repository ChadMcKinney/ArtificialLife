#include <iostream>
#include <time.h>

#include "gameoflife.h"

namespace alife {

namespace cell {

GameOfLife::GameOfLife(unsigned int width, unsigned int height, bool wrapping) :
    AbstractGrid(0, 2, 0, 2, width, height),
    width(width),
    height(height),
    wrapping(wrapping)
{
    initializeGrid();
}

void GameOfLife::update()
{
    for(unsigned int y = 0; y < height; ++y)
    {
        for(unsigned int x = 0; x < width; ++x)
        {
            unsigned int currentState = this->at(x, y);
            unsigned int neighbors = activeNeighbors(x, y);

            if(neighbors < 2 || neighbors > 3)
            {
                // Death
                this->setFuture(0, x, y);
            }

            else if((neighbors == 2 || neighbors == 3) && currentState)
            {
                // Survival
            }

            else if((!currentState) && neighbors == 3)
            {
                // Birth
                this->setFuture(1, x, y);
            }
        }
    }

    AbstractGrid::update(); // Don't forget to update our parent (AFTER) to make the changes permanent
}

void GameOfLife::updateExternalGrid(std::vector< std::vector<unsigned int> >* grid, bool wrapping)
{
    std::vector< std::vector<unsigned int> > gridFuture = *grid;

    for(int x = 0; x < gridFuture.size(); ++x)
    {
        for(int y = 0; y < gridFuture[0].size(); ++y)
        {
            unsigned int currentState = (*grid)[x][y];
            unsigned int neighbors = externalActiveNeighbors(x, y, grid, wrapping);

            if(neighbors < 2 || neighbors > 3)
            {
                // Death
                gridFuture[x][y] = 0;
            }

            else if((neighbors == 2 || neighbors == 3) && currentState)
            {
                // Survival
            }

            else if((!currentState) && neighbors == 3)
            {
                // Birth
                gridFuture[x][y] = 1;
            }
        }
    }

    *grid = gridFuture;
}

unsigned GameOfLife::activeNeighbors(int x, int y)
{
    unsigned int active = 0;

    if(wrapping)
    {
        // Upper left neighbor
        if(this->at(wrapIntRange(x - 1, 0, width), wrapIntRange(y - 1, 0, height)))
            ++active;

        // Upper neigbor
        if(this->at(x, wrapIntRange(y - 1, 0, height)))
            ++active;

        // Upper right neighbor
        if(this->at(wrapIntRange(x + 1, 0, width), wrapIntRange(y - 1, 0, height)))
            ++active;

        // Left neighbor
        if(this->at(wrapIntRange(x - 1, 0, width), y))
            ++active;

        // Right neighbor
        if(this->at(wrapIntRange(x + 1, 0, width), y))
            ++active;

        // Bottom left neighbor
        if(this->at(wrapIntRange(x - 1, 0, width), wrapIntRange(y + 1, 0, height)))
            ++active;

        // Bottom neighbor
        if(this->at(x, wrapIntRange(y + 1, 0, height)))
            ++active;

        // Bottom right neighbor
        if(this->at(wrapIntRange(x + 1, 0, width), wrapIntRange(y + 1, 0, height)))
            ++active;
    }

    if(!wrapping)
    {
        bool notLeftEdge = (x - 1) >= 0;
        bool notTopEdge = (y - 1) >= 0;
        bool notRightEdge = (x + 1) < width;
        bool notBottomEdge = (y + 1) < height;

        // Upper left neighbor
        if(notLeftEdge && notTopEdge)
        {
            if(this->at(x - 1, y - 1))
                ++active;
        }

        // Upper neighbor
        if(notTopEdge)
        {
            if(this->at(x, y - 1))
                ++active;
        }

        // Upper right neighbor
        if(notRightEdge && notTopEdge)
        {
            if(this->at(x + 1, y - 1))
                ++active;
        }

        // Left neighbor
        if(notLeftEdge)
        {
            if(this->at(x - 1, y))
                ++active;
        }

        // Right neighbor
        if(notRightEdge)
        {
            if(this->at(x + 1, y))
                ++active;
        }

        // Bottom legt neighbor
        if(notLeftEdge && notBottomEdge)
        {
            if(this->at(x - 1, y + 1))
                ++active;
        }

        // Bottom neighbor
        if(notBottomEdge)
        {
            if(this->at(x, y + 1))
                ++active;
        }

        // Bottom right neighbor
        if(notRightEdge && notBottomEdge)
        {
            if(this->at(x + 1, y + 1))
                ++active;
        }
    }

    return active;
}

unsigned int GameOfLife::externalActiveNeighbors(int x, int y, std::vector< std::vector<unsigned int> >* grid, bool wrapping)
{
    unsigned int active = 0;
    int gridWidth = grid->size();
    int gridHeight = (*grid)[0].size();

    if(wrapping)
    {
        // Upper left neighbor
        if((*grid)[wrapIntRange(x - 1, 0, gridWidth)][wrapIntRange(y - 1, 0, gridHeight)])
            ++active;

        // Upper neigbor
        if((*grid)[x][wrapIntRange(y - 1, 0, gridHeight)])
            ++active;

        // Upper right neighbor
        if((*grid)[wrapIntRange(x + 1, 0, gridWidth)][wrapIntRange(y - 1, 0, gridHeight)])
            ++active;

        // Left neighbor
        if((*grid)[wrapIntRange(x - 1, 0, gridWidth)][y])
            ++active;

        // Right neighbor
        if((*grid)[wrapIntRange(x + 1, 0, gridWidth)][y])
            ++active;

        // Bottom left neighbor
        if((*grid)[wrapIntRange(x - 1, 0, gridWidth)][wrapIntRange(y + 1, 0, gridHeight)])
            ++active;

        // Bottom neighbor
        if((*grid)[x][wrapIntRange(y + 1, 0, gridHeight)])
            ++active;

        // Bottom right neighbor
        if((*grid)[wrapIntRange(x + 1, 0, gridWidth)][wrapIntRange(y + 1, 0, gridHeight)])
            ++active;
    }

    if(!wrapping)
    {
        bool notLeftEdge = (x - 1) >= 0;
        bool notTopEdge = (y - 1) >= 0;
        bool notRightEdge = (x + 1) < gridWidth;
        bool notBottomEdge = (y + 1) < gridHeight;

        // Upper left neighbor
        if(notLeftEdge && notTopEdge)
        {
            if((*grid)[x - 1][y - 1])
                ++active;
        }

        // Upper neighbor
        if(notTopEdge)
        {
            if((*grid)[x][y - 1])
                ++active;
        }

        // Upper right neighbor
        if(notRightEdge && notTopEdge)
        {
            if((*grid)[x + 1][y - 1])
                ++active;
        }

        // Left neighbor
        if(notLeftEdge)
        {
            if((*grid)[x - 1][y])
                ++active;
        }

        // Right neighbor
        if(notRightEdge)
        {
            if((*grid)[x + 1][y])
                ++active;
        }

        // Bottom legt neighbor
        if(notLeftEdge && notBottomEdge)
        {
            if((*grid)[x - 1][y + 1])
                ++active;
        }

        // Bottom neighbor
        if(notBottomEdge)
        {
            if((*grid)[x][y + 1])
                ++active;
        }

        // Bottom right neighbor
        if(notRightEdge && notBottomEdge)
        {
            if((*grid)[x + 1][y + 1])
                ++active;
        }
    }

    return active;
}

void GameOfLife::initializeGrid()
{
    for(unsigned int y = 0; y < height; ++y)
    {
        for(unsigned int x = 0; x < width; ++x)
        {
            if(randomRange<int>(0, 100) < 3)
                set(1, x, y);
                setFuture(1, x, y); // Set current and future to make our changes permanent
        }
    }
}

} // cell namespace

} // alife namespace
