#include "Trie.h"

#include <algorithm>

// Constructor
Trie::Trie() {
    root = new TrieNode();
}

// Destructor
Trie::~Trie() {
    deleteTrie(root);
}

// Delete allocated nodes
void Trie::deleteTrie(TrieNode* node) {

    if (node == nullptr) {
        return;
    }

    for (auto& child : node->children) {
        deleteTrie(child.second);
    }

    delete node;
}

// Insert word into Trie
void Trie::insert(const std::string& word) {

    if (word.empty()) {
        return;
    }

    TrieNode* current = root;

    for (char ch : word) {

        if (current->children.find(ch)
            == current->children.end()) {

            current->children[ch] = new TrieNode();
        }

        current = current->children[ch];
    }

    current->isEndOfWord = true;

    current->frequency++;
}

// Collect all matching words
void Trie::collectWords(
    TrieNode* node,
    std::string currentWord,
    std::vector<std::pair<std::string, int>>& results
) {

    if (node == nullptr) {
        return;
    }

    if (node->isEndOfWord) {

        results.push_back({
            currentWord,
            node->frequency
        });
    }

    for (auto& child : node->children) {

        collectWords(
            child.second,
            currentWord + child.first,
            results
        );
    }
}

// Find autocomplete suggestions
std::vector<std::string> Trie::getSuggestions(
    const std::string& prefix,
    int limit
) {

    std::vector<std::string> suggestions;

    if (limit <= 0) {
        return suggestions;
    }

    TrieNode* current = root;

    // Locate prefix in Trie
    for (char ch : prefix) {

        if (current->children.find(ch)
            == current->children.end()) {

            return suggestions;
        }

        current = current->children[ch];
    }

    std::vector<std::pair<std::string, int>> results;

    // Collect matching words
    collectWords(current, prefix, results);

    // Sort by frequency, then alphabetically
    std::sort(
        results.begin(),
        results.end(),
        [](const auto& a, const auto& b) {

            if (a.second != b.second) {
                return a.second > b.second;
            }

            return a.first < b.first;
        }
    );

    // Return top suggestions
    for (int i = 0;
         i < std::min(limit, (int)results.size());
         i++) {

        suggestions.push_back(results[i].first);
    }

    return suggestions;
}

// Get frequency of a word
int Trie::getFrequency(const std::string& word) {

    TrieNode* current = root;

    for (char ch : word) {

        if (current->children.find(ch)
            == current->children.end()) {

            return 0;
        }

        current = current->children[ch];
    }

    if (current->isEndOfWord) {
        return current->frequency;
    }

    return 0;
}