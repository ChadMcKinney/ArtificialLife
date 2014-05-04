#ifndef WOLFRAHAMCA_H
#define WOLFRAHAMCA_H

#include "abstractca.h"

namespace alife {

namespace cell {

class WolframCA : public AbstractStream
{

public:

    WolframCA(unsigned int ruleSet = 0, unsigned int possibleStates = 2, unsigned int initialState = 1,
              unsigned int size = 1025);

    void advance();

protected:

};

} // cell namespace

} // alife namespace

#endif // WOLFRAHAMCA_H
