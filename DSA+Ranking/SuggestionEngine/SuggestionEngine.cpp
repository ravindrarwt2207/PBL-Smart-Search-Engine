#include "SuggestionEngine.h"

SuggestionEngine::SuggestionEngine() {
    // Constructor - Trie is automatically created
}

void SuggestionEngine::addSuggestion(const std::string& text, int frequency) {
    if (text.empty()) return;
    trie.insert(text, frequency);
}

int SuggestionEngine::getSuggestions(const std::string& partialQuery, Suggestion results[], int limit) {
    if (partialQuery.empty() || limit <= 0) {
        return 0;
    }

    return trie.getSuggestions(partialQuery, results, limit);
}

void SuggestionEngine::clear() {
    trie.clear();
}
