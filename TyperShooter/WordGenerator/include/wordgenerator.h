#ifndef GENERATOR_H
#define GENERATOR_H
#include <vector>
#include <string>
#include "partitioner.h"
#include "strictpartitioner.h"
#include "textreader.h"

// A class for generating random words based on any partition
class WordGenerator {
    std::vector<std::string> words;

public:
    WordGenerator();

    // Create a generator with a vector of words
    WordGenerator(std::vector<std::string> words);

    // Returns a random word
    std::string randWord();

    // Returns a random word that appears in the given partition
    std::string randWordFrom(Partitioner partition);

    // Returns a random word that appears in any of the given partitions
    std::string randUnionedWordFrom(std::vector<Partitioner> partitions);

    // Returns a random word that appears in all of the given partitions
    std::string randIntersectedWordFrom(std::vector<Partitioner> partitions);
};

#endif // TEXTREADER_H
