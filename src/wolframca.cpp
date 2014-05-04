#include <iostream>

#include "wolframca.h"

namespace alife {

namespace cell {


WolframCA::WolframCA(unsigned int ruleSet, unsigned int possibleStates, unsigned int initialState, unsigned int size):
    AbstractStream(ruleSet, possibleStates, initialState, size)
{

}

void WolframCA::advance()
{
    if(possibleStates == 2) // Binary states
    {
        for(int i = 0; i < futureState.size(); ++i) // iterate over the values to produce the next generation
        {
            int neighbors[3]; // Create the neighbor array, then populate it

            if(i - 1 < 0)
                neighbors[2] = 0;
            else
                neighbors[2] = currentState[i - 1];

            neighbors[1] = currentState[i];

            if(i + 1 >= currentState.size())
                neighbors[0] = 0;
            else
                neighbors[0] = currentState[i + 1];

            int index = 0; // Used to index the ruleArray

            // convert to an index
            for(int j = 0; j < 3; ++j)
            {
                index += neighbors[j] << j;
            }

            index = abs(ruleArraySize - 1 - index);
            futureState[i] = ruleArray[index];
        }
    }

    else
    {
        for(int i = 0; i < futureState.size(); ++i)
        {
            double average = 0;

            if(i - 1 >= 0)
                average += currentState[i - 1];

            average += currentState[i];

            if(i + 1 < currentState.size())
                average += currentState[i + 1];

            average /= 3;
            futureState[i] = ruleMap[average];
        }
    }

    AbstractStream::advance(); // Remember to call parent's advance when we're done
}

} // cell namespace

} // alife namespace
