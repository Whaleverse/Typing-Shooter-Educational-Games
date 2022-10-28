#ifndef TEXTREADER_H
#define TEXTREADER_H
#include <iostream>
#include <fstream>
#include <vector>

class TextReader {
    std::vector<std::string> dictionary;

public:
    TextReader (std::string filename);

    // Returns a random word
    std::string pickARandomWord();

    // Returns a vector of all the words read in
    std::vector<std::string> getWords();
};

#endif // TEXTREADER_H
