#include "abstractca.h"

#include <iostream>
#include <set>
#include <cmath>
#include <cstdarg>
#include <cstring>

namespace alife {

namespace cell {

////////////////////
// AbstractAutomata
////////////////////

AbstractAutomata::AbstractAutomata(unsigned int ruleSet, unsigned int possibleStates) :
    ruleSet(ruleSet),
    possibleStates(possibleStates),
    iteration(0)
{

}

void AbstractAutomata::update()
{
    ++iteration;
}

unsigned int AbstractAutomata::getRuleSet()
{
    return ruleSet;
}

void AbstractAutomata::setRuleSet(unsigned ruleSet)
{
    this->ruleSet = ruleSet;
}

unsigned int AbstractAutomata::getPossibleStates()
{
    return possibleStates;
}

void AbstractAutomata::setPossible(unsigned possibleStates)
{
    this->possibleStates = possibleStates;
}

////////////////
// AbstractGrid
////////////////

AbstractGrid::AbstractGrid(unsigned int ruleSet, unsigned int possibleStates, unsigned int initialState,
                           std::vector<unsigned int> &extents) :
    AbstractAutomata(ruleSet, possibleStates),
    extents(extents)
{
    init(initialState);
}

AbstractGrid::AbstractGrid(unsigned int ruleSet, unsigned int possibleStates, unsigned int initialState, unsigned int dimensions,
                           unsigned int firstExtent, ...) :
    AbstractAutomata(ruleSet, possibleStates),
    dimensions(dimensions)
{
    va_list vl;
    va_start(vl, firstExtent);
    extents.push_back(firstExtent);

    for(int i = 1; i < dimensions; ++i)
    {
        extents.push_back(va_arg(vl, int));
    }

    va_end(vl);

    init(initialState);
}

void AbstractGrid::init(unsigned int initialState)
{
    createArray();
    fill(initialState);
}

AbstractGrid::~AbstractGrid()
{
    deleteArray();
}

void AbstractGrid::update()
{
    memcpy(gridArray, futureGridArray, sizeof(unsigned int) * length);
    AbstractAutomata::update(); // Don't forget to update our parent
}

unsigned int AbstractGrid::at(int index, ...)
{
    va_list vl;
    va_start(vl, index);
    unsigned int indexes[dimensions];
    indexes[0] = index;

    for(int i = 1; i < dimensions; ++i)
    {
        indexes[i] = va_arg(vl, int);
    }

    va_end(vl);

    return gridArray[interleaveIndex(indexes)];
}

unsigned int AbstractGrid::operator ()(int index, ...)
{
    va_list vl;
    va_start(vl, index);
    unsigned int indexes[dimensions];
    indexes[0] = index;

    for(int i = 1; i < dimensions; ++i)
    {
        indexes[i] = va_arg(vl, int);
    }

    va_end(vl);

    return gridArray[interleaveIndex(indexes)];
}

void AbstractGrid::set(unsigned int value , int index, ...)
{
    va_list vl;
    va_start(vl, index);
    unsigned int indexes[dimensions];
    indexes[0] = index;

    for(int i = 1; i < dimensions; ++i)
    {
        indexes[i] = va_arg(vl, int);
    }

    va_end(vl);

    gridArray[interleaveIndex(indexes)] = value;
}

void AbstractGrid::setFuture(unsigned int value , int index, ...)
{
    va_list vl;
    va_start(vl, index);
    unsigned int indexes[dimensions];
    indexes[0] = index;

    for(int i = 1; i < dimensions; ++i)
    {
        indexes[i] = va_arg(vl, int);
    }

    va_end(vl);

    futureGridArray[interleaveIndex(indexes)] = value;
}

void AbstractGrid::fill(unsigned int state)
{
    for(int i = 0; i < length; ++i)
    {
        futureGridArray[i] = state;
    }

    update();
}

void AbstractGrid::randomize()
{

    for(int i = 0; i < length; ++i)
    {
        unsigned int rand = randomRange<unsigned int>(0, possibleStates) - 1;
        futureGridArray[i] = rand;
    }

    update();
}

unsigned int AbstractGrid::interleaveIndex(unsigned int* indexes)
{
    if(indexes[0] >= extents[0])
        indexes[0] = extents[0] - 1;

    unsigned int index = indexes[0];

    for(int i = 1; i < dimensions; ++i)
    {
        if(indexes[i] > extents[i])
        {
            std::cerr << "Index: " << indexes[i] << " exceeds bouds. redeclaring to " << extents[i] - 1 << std::endl;
            indexes[i] = extents[i] - 1;
        }

        int cursor = indexes[i];

        for(int j = 0; j < i; ++j)
        {
            cursor *= extents[j];
        }

        index += cursor;
    }

    return index;
}

const std::vector<int>& AbstractGrid::getExtents()
{

}

unsigned int* AbstractGrid::deinterleaveIndex(unsigned int index)
{
    unsigned int indexes[dimensions];

    if(index >= length)
    {
        std::cerr << "Index: " << index << " exceeds bouds. redeclaring to " << length - 1 << std::endl;
        index = length - 1;
    }

    for(int i = dimensions; i > 0; --i)
    {
        unsigned int cursor = index;

        for(int j = 0; j < i; ++j)
        {
            cursor /= extents[j];
        }

        indexes[i] = cursor;

        if(cursor > 0)
        {
            for(int j = 0; j < i; ++j)
            {
                cursor *= extents[j];
            }

            index -= cursor;
        }
    }

    indexes[0] = index;

    return indexes;
}

bool AbstractGrid::runUnitTest()
{
    bool pass = true;

    for(int i = 0; i < length; ++i)
    {
        unsigned int* indexes = deinterleaveIndex(i);
        unsigned int index = interleaveIndex(indexes);

        bool currentIndexPass = index == i;

        if(!currentIndexPass)
        {
            std::cout << std:: endl << "FAIL. Correct index: " << i << " Translated index: " << index << std::endl;
            std::cout << "Translated to grid coordinates: ";

            for(int l = 0; l < dimensions; ++l)
            {
                std::cout << indexes[l];
                if(l < dimensions - 1)
                    std::cout << ", ";
                else
                    std::cout << "] :";
            }

            std::cout << std::endl;
            pass = false;
        }

    }

    std::cout << "Grid Unit test: " << pass << std::endl;
    return pass;
}

void AbstractGrid::print()
{
    std::cout << "Dimensions: " << dimensions << " Extents size: " << extents.size() << " extents enumerated: [";

    for(int i = 0; i < extents.size(); ++i)
    {
        std::cout << extents[i];

        if(i < extents.size() - 1)
            std::cout << ", ";
        else
            std::cout << "]" << std::endl;
    }

    std::cout << "[";

    for(int i = 0; i < length; ++i)
    {
        std::cout << gridArray[i];

        if(i < length - 1)
            std::cout << ", ";
        else
            std::cout << "]";
    }

    std::cout << std::endl;
}

void AbstractGrid::createArray()
{
    dimensions = extents.size();
    length = extents.front();

    if(dimensions > 1)
    {
        for(int i = 1; i < dimensions; ++i)
        {
            length *= extents[i];
        }
    }

    gridArray = new unsigned int[length];
    futureGridArray = new unsigned int[length];

    for(int i = 0; i < length; ++i)
    {
        gridArray[i] = 0;
        futureGridArray[i] = 0;
    }
}

void AbstractGrid::deleteArray()
{
    delete gridArray;
}

void AbstractGrid::initializeGrid()
{

}

//////////////////
// AbstractStream
//////////////////

AbstractStream::AbstractStream(unsigned int ruleSet, unsigned int possibleStates, unsigned int initialState, unsigned int size) :
    AbstractAutomata(ruleSet, possibleStates)
{
    if(size % 2 == 0) // if even, make odd
    {
        this->size = size - 1;
        //std::cout << "SIZE INPUT FOR CELLULAR AUTOMATA STREAM IS EVEN, CHANGING TO: " << this->size << std::endl;
    }

    else
    {
        size = size;
    }

    createRuleArray();
    for(int i = 0; i < size; ++i)
    {
        currentState.push_back(0);
        futureState.push_back(0);
    }

    int index = (size / 2) + 0.5;
    currentState[index] = initialState; // Set initial state
}

void AbstractStream::advance() // Child is responsible for updating the future state
{
    currentState = futureState;
    ++iteration;
}

const std::vector<unsigned int>& AbstractStream::getCurrentState()
{
    return currentState;
}

void AbstractStream::operator ++()
{
    advance();
}

void AbstractStream::calculateRuleArraySize()
{
    std::set<double> uniqueAverages;
    unsigned int neighbors = 3;
    unsigned int stateArray[neighbors];

    for(int i = 0; i < neighbors; ++i)
    {
        stateArray[i] = 0;
    }

    for(int i = 0; i < neighbors; ++i)
    {
        for(int j = 0; j < possibleStates; ++j)
        {
            stateArray[i] = j;
            double average = 0;

            for(int k = 0; k < neighbors; ++k)
            {
                average += stateArray[k];
            }

            average /= neighbors;

            uniqueAverages.insert(average);
        }
    }

    ruleArraySize = uniqueAverages.size();

    // Populate the rule averages vector
    std::set<double>::iterator iter = uniqueAverages.begin();
    while(iter != uniqueAverages.end())
    {
        ruleAverages.push_back(*iter);
        ++iter;
    }

    // Sort the vector
    std::sort(ruleAverages.begin(), ruleAverages.end());
    //std::cout << "Rule Array Size: " << ruleArraySize << std::endl;
}

void AbstractStream::createRuleArray()
{
    if(possibleStates == 2)
    {
        ruleArraySize = 8;

        for(int i = 0; i < ruleArraySize; ++i)
        {
            unsigned int rule = (ruleSet >> (ruleArraySize - i - 1)) & 1;
            ruleArray.push_back(rule);
        }

        /*
        std::cout << "Rule Array: [";

        for(int i = 0; i < ruleArraySize; ++i)
        {
            std::cout << ruleArray[i];
            if(i < ruleArraySize - 1)
                std::cout << ", ";
            else
                std::cout << "]" << std::endl;
        }*/
    }

    else
    {
        calculateRuleArraySize();

        for(int i = ruleArraySize - 1; i >= 0; --i)
        {
            unsigned int rule = (unsigned int) (ruleSet / pow(possibleStates, i)) % possibleStates;
            ruleArray.push_back(rule);
            ruleMap[ruleAverages[i]] =  rule; // Map for extracting the correct rule
        }

        boost::unordered::unordered_map<double, unsigned int>::iterator iter = ruleMap.begin();

        /*
        std::cout << "Rule Map: [ ";
        while(iter != ruleMap.end())
        {
            std::cout << iter->first << " ";
            ++iter;
        }

        std::cout << "]" << std::endl;

        std::cout << "Rule Array: [";

        for(int i = 0; i < ruleArraySize; ++i)
        {
            std::cout << ruleArray[i];
            if(i < ruleArraySize - 1)
                std::cout << ", ";
            else
                std::cout << "]" << std::endl;
        }*/
    }
}

} // cell namespace

} // alife namespace
