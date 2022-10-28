#ifndef TYPERWORDGENERATOR_H
#define TYPERWORDGENERATOR_H
#include <string>
#include "strictpartitioner.h"
#include "partitioner.h"
#include "wordgenerator.h"
#include "textreader.h"


class TyperWordGenerator
{
    std::vector<StrictPartitioner> levels;
    WordGenerator generator;
    int currentLevel;

public:
    TyperWordGenerator(std::string dictionaryLocation);

    // Returns the current level
    int getCurrentLevel();

    // Resets the difficulty of the words returned to the lowest level
    void resetDifficulty();

    // Increases the difficulty of the words returned
    void increaseDifficulty();

    // Returns a random word at the appropriate level
    std::string randomWord();
};

#endif // TYPERWORDGENERATOR_H
