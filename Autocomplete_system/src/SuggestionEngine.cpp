#include "SuggestionEngine.h"

// Add initial vocabulary
void SuggestionEngine::addWords(
    const std::vector<std::string>& words
) {

    for (const std::string& word : words) {
        trie.insert(word);
    }
}

// Update vocabulary with newly indexed words
void SuggestionEngine::updateVocabulary(
    const std::vector<std::string>& newWords
) {

    for (const std::string& word : newWords) {
        trie.insert(word);
    }
}

// Generate autocomplete suggestions
std::vector<std::string> SuggestionEngine::suggest(
    const std::string& prefix,
    int limit
) {

    return trie.getSuggestions(prefix, limit);
}

// Get frequency of a word
int SuggestionEngine::getWordFrequency(
    const std::string& word
) {

    return trie.getFrequency(word);
}