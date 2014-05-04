#ifndef ABSTRACTCA_H
#define ABSTRACTCA_H

#include <vector>
#include <deque>
#include <math.h>
#include <iostream>
#include <fstream>
#include "boost/unordered_map.hpp"

namespace alife {

namespace cell {

template <class T>
inline T randomRange(T minValue, T maxValue)
{
    return minValue + (rand() % (T)(maxValue - minValue + 1));
}

inline int wrapIntRange(int inValue, int minValue, int maxValue)
{
    if(inValue < minValue)
        return maxValue - (minValue - inValue);

    else if (inValue > maxValue)
        return 0;
        //return (inValue % maxValue) + minValue;

    else return inValue;
}

////////////////////
// AbstractAutomata
////////////////////

class AbstractAutomata // Parent class that all CA classes inherit from
{
public:

    /**
    * AbstractAutomata an abstract class for creating and iterating cellular automata.
    * @param unsigned int The ruleset used for the automata
    * @param unsigned int The number of possible states the automata can be in. 2 is binary, ie. On or Off
    */
    AbstractAutomata(unsigned int ruleSet = 0, unsigned int possibleStates = 2);

    virtual void update();

    virtual unsigned int getRuleSet();
    virtual void setRuleSet(unsigned ruleSet);
    virtual unsigned int getPossibleStates();
    virtual void setPossible(unsigned possibleStates);

    unsigned int iteration;

protected:

    unsigned int ruleSet, possibleStates;
};

////////////////
// AbstractGrid
////////////////

// Parent class that grid based CA classes inherit from
class AbstractGrid : public AbstractAutomata
{
public:

    /**
    * AbstractGrid an abstract class for creating automata that exist on a grid in more than one dimensions.
    * @param unsigned int The ruleSet, if any, used for the automata
    * @param unsigned int The number of possible states the automata can be in. 2 is binary, ie. On or Off
    * @param unsigned int The initial state of all the cells.
    * @param vector<unsigned int>& a vector containing the extents for each dimenions. Ex [20, 20, 20] for 3 dimensions
    */
    AbstractGrid(unsigned int ruleSet, unsigned int possibleStates, unsigned int initialState, std::vector<unsigned int>& extents);

    /**
    * AbstractGrid an abstract class for creating automata that exist on a grid in more than one dimensions.
    * @param unsigned int The ruleSet, if any, used for the automata
    * @param unsigned int The number of possible states the automata can be in. 2 is binary, ie. On or Off
    * @param unsigned int The initial state of all the cells.
    * @param unsigned int Takes a dimensions number followed by that number of extents.
    *    Ex: 3, 20, 20, 20 or 2, 100, 100 or 5, 55, 44, 33, 22, 11
    */
    AbstractGrid(unsigned int ruleSet, unsigned int possibleStates, unsigned int initialState, unsigned int dimensions,
                 unsigned int firstExtent, ...);

    ~AbstractGrid();

    void update(); // Overide this!

    unsigned int at(int index, ...);
    unsigned int operator ()(int index, ...);
    virtual void set(unsigned int value , int index, ...);
    virtual void setFuture(unsigned int value , int index, ...);
    virtual void fill(unsigned int state);
    virtual void randomize();

    unsigned int length;
    unsigned int dimensions;
    const std::vector<int>& getExtents();

    unsigned int interleaveIndex(unsigned int* indexes); // flatten multidimensional index to a single dimension
    unsigned int* deinterleaveIndex(unsigned int index); // expand single dimension index to a multidimensional index

    bool runUnitTest(); // Check the sanity of our container, returns pass / fail flag
    void print(); // Warning, don't use this on large collections or it will take a looooong time.

protected:

    void init(unsigned int initialState = 0);
    void createArray();
    void deleteArray();

    virtual void initializeGrid() = 0; // Override this!

    unsigned int* gridArray;
    unsigned int* futureGridArray;
    std::vector<unsigned int> extents;
};

////////////
// Automata
////////////

class Automata // Class for containing Stream history of Automata produced by AbstractStream classes
{
public:

    typedef std::deque< std::vector<unsigned int> >::iterator auto_iter_t;
    std::deque< std::vector<unsigned int> >history;
};

/////////////////
// AbstractStream
/////////////////

class AbstractStream : public AbstractAutomata
{
public:

    /**
    * Abstract Stream
    * @param unsigned int The ruleset used for this iteration of the CA.
    * @param unsigned int The number of possible states for each automaton.
    * @param unsigned int The Size for the vector used to contain CA generations. Must be odd.
    */
    AbstractStream(unsigned int ruleSet = 0, unsigned int possibleStates = 2,
                   unsigned int initialState = 1, unsigned int size = 1025);

    void advance();  // Child is responsible for updating the future state
    void operator ++(); // Same as advance
    const std::vector<unsigned int>& getCurrentState();

    unsigned int size;
    unsigned int ruleArraySize;

protected:

    void calculateRuleArraySize();
    void createRuleArray();

    boost::unordered::unordered_map<double, unsigned int> ruleMap;
    std::vector<double> ruleAverages; // Collection of averages for CA with more than 3 possible states
    std::vector<unsigned int> futureState, currentState, ruleArray; // State vectors
};

} // cell namespace

} // alife namespace


#endif // ABSTRACTCA_H
