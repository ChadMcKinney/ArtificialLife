#ifndef LSYSTEM_H
#define LSYSTEM_H

// System includes
#include <vector>
#include <string>

namespace alife {

extern char alphabet[6];

enum AlphabetIndexes
{
    F,
    G,
    Plus,
    Minus,
    Save,
    Restore
};

////////
// Rule
////////

struct Rule {

    char a;
    std::string b;

    Rule(char a, std::string b) : a(a), b(b) {}
};

typedef std::vector<Rule> rule_set;

///////////
// LSystem
///////////

class LSystem {

public:
    LSystem(std::string axiom, std::vector<Rule> ruleSet);

    void generate();
    std::string getSentence();
    int getGeneration();

private:

    int generation;
    std::string sentence;
    std::vector<Rule> ruleSet;
};

////////////////////////////
// Fractal Implementations
////////////////////////////

class SierpinksiGasketTriangle {

public:
    SierpinksiGasketTriangle();
    ~SierpinksiGasketTriangle();

    void generate();
    std::string getSentence();
    int getGeneration();

private:

    LSystem* lSystem;
};

} // fractal namespace

#endif // LSYSTEM_H
