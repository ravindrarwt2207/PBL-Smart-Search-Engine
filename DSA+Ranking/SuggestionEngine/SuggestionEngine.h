#ifndef SUGGESTIONENGINE_H
#define SUGGESTIONENGINE_H

#include <string>
#include "../Trie/Trie.h"
#include "../Common/Common.h"

class SuggestionEngine {
private:
    Trie trie;

public:
    SuggestionEngine();

    // Add a single suggestion
    void addSuggestion(const std::string& text, int frequency = 1);

    // Get top suggestions
    // results[] = array to store suggestions
    // limit = maximum number of suggestions (usually 5)
    // Returns the number of suggestions found
    int getSuggestions(const std::string& partialQuery, Suggestion results[], int limit = 5);

    // Clear all suggestions
    void clear();
};

#endif
