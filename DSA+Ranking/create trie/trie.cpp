#include "Trie.h"
#include <algorithm>
#include <cctype>

Trie::Trie() {
    root = new TrieNode();
}

Trie::\~Trie() {
    clear();
    delete root;
}

void Trie::clearNode(TrieNode* node) {
    if (node == nullptr) return;

    for (auto& pair : node->children) {
        clearNode(pair.second);
        delete pair.second;
    }
    node->children.clear();
}

void Trie::clear() {
    clearNode(root);
    root->isEndOfWord = false;
    root->frequency = 0;
    root->word.clear();
}

void Trie::insert(const std::string& word, int freq) {
    if (word.empty()) return;

    TrieNode* current = root;

    for (char c : word) {
        char lower = std::tolower(static_cast<unsigned char>(c));

        if (current->children.find(lower) == current->children.end()) {
            current->children[lower] = new TrieNode();
        }
        current = current->children[lower];
    }

    current->isEndOfWord = true;
    current->frequency += freq;
    current->word = word;
}

bool Trie::search(const std::string& word) {
    TrieNode* current = root;

    for (char c : word) {
        char lower = std::tolower(static_cast<unsigned char>(c));

        if (current->children.find(lower) == current->children.end()) {
            return false;
        }
        current = current->children[lower];
    }
    return current->isEndOfWord;
}

bool Trie::startsWith(const std::string& prefix) {
    TrieNode* current = root;

    for (char c : prefix) {
        char lower = std::tolower(static_cast<unsigned char>(c));

        if (current->children.find(lower) == current->children.end()) {
            return false;
        }
        current = current->children[lower];
    }
    return true;
}

void Trie::collectSuggestions(TrieNode* node, std::vector<Suggestion>& results, int limit) {
    if (node == nullptr || results.size() >= static_cast<size_t>(limit)) return;

    if (node->isEndOfWord) {
        results.push_back(Suggestion(node->word, node->frequency));
    }

    for (auto& pair : node->children) {
        if (results.size() >= static_cast<size_t>(limit)) break;
        collectSuggestions(pair.second, results, limit);
    }
}

std::vector<Suggestion> Trie::getSuggestions(const std::string& prefix, int limit) {
    std::vector<Suggestion> results;

    if (prefix.empty() || limit <= 0) return results;

    TrieNode* current = root;

    for (char c : prefix) {
        char lower = std::tolower(static_cast<unsigned char>(c));

        if (current->children.find(lower) == current->children.end()) {
            return results;
        }
        current = current->children[lower];
    }

    // Collect more suggestions first, then sort
    collectSuggestions(current, results, limit * 3);

    // Sort by frequency (highest first)
    std::sort(results.begin(), results.end(), [](const Suggestion& a, const Suggestion& b) {
        return a.frequency > b.frequency;
    });

    // Keep only top 'limit' suggestions
    if (results.size() > static_cast<size_t>(limit)) {
        results.resize(limit);
    }

    return results;
}
