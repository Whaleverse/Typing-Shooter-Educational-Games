#include "textreader.h"
#include <iostream>
#include <ctime>
#include <fstream>

// Returns a random word
TextReader::TextReader(std::string filename) {
    srand(time(NULL)); // seed the random word generator
    std::ifstream fileReader;
    fileReader.open(filename);
    if (fileReader.is_open()) {
        while (!fileReader.eof()) {
            std::string word;
            fileReader >> word;
            dictionary.push_back(word);
        }
    }
    fileReader.close();
}

// Returns a vector of all the words read in
std::string TextReader::pickARandomWord() {
    return dictionary.at(unsigned(rand()) % dictionary.size());
}

// Returns a vector of all the words read in
std::vector<std::string> TextReader::getWords() {
    return dictionary;
}
