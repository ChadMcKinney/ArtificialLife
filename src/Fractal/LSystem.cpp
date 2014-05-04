#include "LSystem.h"

namespace fractal {

char alphabet[6] = { 'F', 'G', '+', '-', '[', ']'};

///////////
// LSystem
///////////

LSystem::LSystem(std::string axiom, std::vector<Rule> ruleSet) :
    sentence(axiom),
    ruleSet(ruleSet),
    generation(0)
{

}

void LSystem::generate()
{
    std::string nextGeneration;

    for(int i = 0; i < sentence.length(); ++i)
    {
        char currentChar = sentence.at(i);
        std::string replace(1, currentChar);

        for(int j = 0; j < ruleSet.size(); ++j)
        {
            char a = ruleSet[j].a;
            if(a == currentChar)
            {
                replace = ruleSet[j].b;
                break;
            }
        }

        nextGeneration.append(replace);
    }

    sentence = nextGeneration;
    ++generation;
}

std::string LSystem::getSentence()
{
    return sentence;
}

int LSystem::getGeneration()
{
    return generation;
}

//////////////////////////////
// SierpinksiGasketTriangle
//////////////////////////////

SierpinksiGasketTriangle::SierpinksiGasketTriangle()
{
    std::vector<Rule> ruleSet;
    ruleSet.push_back(Rule('F', "F--F--F--G"));
    ruleSet.push_back(Rule('G', "GG"));
    lSystem = new LSystem("F--F--F", ruleSet);
}

SierpinksiGasketTriangle::~SierpinksiGasketTriangle()
{
    delete lSystem;
}

void SierpinksiGasketTriangle::generate()
{
    lSystem->generate();
}

std::string SierpinksiGasketTriangle::getSentence()
{
    return lSystem->getSentence();
}

int SierpinksiGasketTriangle::getGeneration()
{
    return lSystem->getGeneration();
}

} // fractal namespace
