#include "wordgenerator.h"
#include <random>
#include <ctime>

WordGenerator::WordGenerator()
{
    words = std::vector<std::string> {};
    srand(time_t(0));
}

// Create a generator with a vector of words
WordGenerator::WordGenerator(std::vector<std::string> words) :words(words)
{
    srand(time_t(0));
}

// Returns a random word
std::string WordGenerator::randWord()
{
    std::string randWord = "";
    if (words.size() > 0)
        randWord = words.at(unsigned(rand()) % words.size());
    return randWord;
}

// Get a random word from the partition
std::string WordGenerator::randWordFrom(Partitioner partition)
{
    std::vector<int> usedIndexes;
    std::string randWord = "";

    // Iterate through all the words if we can't find a applicable one
    while (usedIndexes.size() <= words.size()) {
        int randIndex = int(unsigned(rand()) % words.size());
        // If we've already used this index
        if (std::find(usedIndexes.begin(), usedIndexes.end(), randIndex) != usedIndexes.end())
            continue;
        randWord = words.at(unsigned(rand()) % words.size());

        if (partition.contains(randWord))
            return randWord;
        else
            usedIndexes.push_back(randIndex);
    }
    return randWord;

}

// Returns a random word that appears in any of the given partitions. Returns a blank
// string if there are no items in any of the partitions.
std::string WordGenerator::randUnionedWordFrom(std::vector<Partitioner> partitions)
{
    std::vector<int> usedIndexes;
    std::string randWord = "";

    // Iterate through all the words if we can't find a applicable one
    while (usedIndexes.size() <= words.size()) {
        int randIndex = int(unsigned(rand()) % words.size());
        // If we've already used this index
        if (std::find(usedIndexes.begin(), usedIndexes.end(), randIndex) != usedIndexes.end())
            continue;
        randWord = words.at(unsigned(rand()) % words.size());

        for (auto partition : partitions) {
            if (partition.contains(randWord))
                return randWord;
            else
                usedIndexes.push_back(randIndex);
        }
    }
    return randWord;
}

// Returns a random word that appears in all of the given partitions. Returns a blank
// string if there are no items in all of the partitions.
std::string WordGenerator::randIntersectedWordFrom(std::vector<Partitioner> partitions)
{
    std::vector<int> usedIndexes;
    std::string randWord = "";

    // Iterate through all the words if we can't find a applicable one
    while (usedIndexes.size() <= words.size()) {
        int randIndex = int(unsigned(rand()) % words.size());
        // If we've already used this index
        if (std::find(usedIndexes.begin(), usedIndexes.end(), randIndex) != usedIndexes.end())
            continue;
        randWord = words.at(unsigned(rand()) % words.size());

        bool allTrue = true;
        // If word is in all partitions
        for (auto partition : partitions) {
            if (partition.contains(randWord)) {
                allTrue = false;
                break;
            }
            usedIndexes.push_back(randIndex);
        }

        if (allTrue) return randWord;
    }
    return randWord;
}

