#include "partitioner.h"
#include "wordgenerator.h"
#include <iostream>
#include <assert.h>

// Is a character in the word
bool isCharInWord(std::string word, char c) {
    return word.find(c) != std::string::npos;
}

void assertBasicRule() {
    std::vector<std::string> testStrings {"apple", "cat", "dog", "hippo", "", "-", "a"};
    Partitioner partition( [](std::string word){ return isCharInWord(word, 'a'); } );
    std::vector<int> expectedValues = {true, true, false, false, false, false, true};
    int expIndex = 0;
    for (auto testStr : testStrings)
        assert(partition.contains(testStr) == expectedValues.at(expIndex++));
}

void assertAppendRule() {
    std::vector<std::string> testStrings {"apple", "cat", "dog", "hippo", "", "-", "a"};
    Partitioner partition( [](std::string word){ return isCharInWord(word, 'a'); } );
    partition.addRule( [](std::string word){ return isCharInWord(word, 'o'); } );
    std::vector<int> expectedValues = {true, true, true, true, false, false, true};
    int expIndex = 0;
    for (auto testStr : testStrings)
        assert(partition.contains(testStr) == expectedValues.at(expIndex++));
}

void assertBasicPartition() {
    std::vector<std::string> testStrings {"apple", "cat", "dog", "hippo", "", "-", "a"};
    Partitioner partition( [](std::string word){ return isCharInWord(word, 'h'); } );
    assert(partition.partition(testStrings).at(0) == "hippo");
}

void assertRandWordFromGenerator() {
    std::vector<std::string> testStrings {"apple", "cat", "dog", "hippo", "", "-", "a"};
    Partitioner partition( [](std::string word){ return isCharInWord(word, 'h'); } );
    WordGenerator generator(testStrings);
    assert(generator.randUnionedWordFrom(std::vector<Partitioner> {partition} ) == "hippo");
}

void assertBasicStrictPartition() {
    std::vector<std::string> testStrings {"apple", "cat", "dog", "hippo", "", "-", "a"};
    Partitioner partition( [](std::string word){ return isCharInWord(word, 'o'); } );
    partition.addRule( [](std::string word){ return isCharInWord(word, 'd'); } );
    assert(partition.partition(testStrings).at(0) == "dog");
}

// A poor mans test suite
int main() {
    assertBasicRule();
    assertAppendRule();
    assertBasicPartition();
    assertRandWordFromGenerator();
};
