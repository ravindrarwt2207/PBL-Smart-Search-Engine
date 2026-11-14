#ifndef SUGGESTIONENGINE_H
#define SUGGESTIONENGINE_H

#include "Trie.h"
#include <string>
#include <vector>

class SuggestionEngine {

private:

    Trie trie;

public:

    // Add multiple words
    void addWords(
        const std::vector<std::string>& words
    );

    // Add words from newly indexed documents
    void updateVocabulary(
        const std::vector<std::string>& newWords
    );

    // Generate suggestions
    std::vector<std::string> suggest(
        const std::string& prefix,
        int limit = 5
    );

    // Get frequency of a word
    int getWordFrequency(
        const std::string& word
    );

};

#endif