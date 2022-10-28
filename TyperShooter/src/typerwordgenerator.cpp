#include "typerwordgenerator.h"

bool levelOneRule(std::string word)
{
    return word.length() < 10;
}

bool levelTwoRule(std::string word)
{
    return word.length() > 5 && word.length() < 10;
}

bool levelThreeRule(std::string word)
{
    return word.length() > 10;
}

TyperWordGenerator::TyperWordGenerator(std::string dictionaryLocation)
{
   TextReader reader(dictionaryLocation);
   generator = WordGenerator(reader.getWords());
   levels = std::vector<StrictPartitioner> { StrictPartitioner(levelOneRule),
                                             StrictPartitioner(levelTwoRule),
                                             StrictPartitioner(levelThreeRule) };
   currentLevel = 0;
}

// Returns the current level
int TyperWordGenerator::getCurrentLevel()
{
    return currentLevel + 1;
}

// Resets the difficulty of the words returned to the lowest level
void TyperWordGenerator::resetDifficulty()
{
    currentLevel = 0;
}

// Increases the difficulty of the words returned
void TyperWordGenerator::increaseDifficulty()
{
    if (currentLevel < int(levels.size()) - 1)
        currentLevel++;
}

// Returns a random word at the appropriate level
std::string TyperWordGenerator::randomWord()
{
    StrictPartitioner currentPartition(levels.at(unsigned(currentLevel)));
    return generator.randWordFrom(currentPartition);
}
