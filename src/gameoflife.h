#ifndef GAMEOFLIFE_H
#define GAMEOFLIFE_H

#include "abstractca.h"

namespace alife {

namespace cell {

class GameOfLife : public AbstractGrid
{
public:

    GameOfLife(unsigned int width, unsigned int height, bool wrapping = false);

    virtual void update();
    static void updateExternalGrid(std::vector< std::vector<unsigned int> >* grid, bool wrapping = false);

    unsigned int width;
    unsigned int height;
    bool wrapping;

    static const unsigned int numNeighbors = 8;

protected:

    unsigned int activeNeighbors(int x, int y);
    static unsigned int externalActiveNeighbors(int x, int y, std::vector< std::vector<unsigned int> >* grid, bool wrapping);
    void initializeGrid();
};

} // cell namespace

} // alife namespace

#endif // GAMEOFLIFE_H
