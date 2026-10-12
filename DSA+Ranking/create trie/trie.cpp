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

void Trie::collectSuggestions(TrieNode* node, Suggestion results[], int& count, int limit) {
    if (node == nullptr || count >= limit) return;

    if (node->isEndOfWord) {
        results[count] = Suggestion(node->word, node->frequency);
        count++;
    }

    for (auto& pair : node->children) {
        if (count >= limit) break;
        collectSuggestions(pair.second, results, count, limit);
    }
}

int Trie::getSuggestions(const std::string& prefix, Suggestion results[], int limit) {
    int count = 0;

    if (prefix.empty() || limit <= 0) return 0;

    TrieNode* current = root;

    for (char c : prefix) {
        char lower = std::tolower(static_cast<unsigned char>(c));

        if (current->children.find(lower) == current->children.end()) {
            return 0;
        }
        current = current->children[lower];
    }

    // Temporary array to collect more suggestions
    Suggestion temp[50];
    int tempCount = 0;

    collectSuggestions(current, temp, tempCount, 50);

    // Simple sorting by frequency (descending)
    for (int i = 0; i < tempCount - 1; i++) {
        for (int j = i + 1; j < tempCount; j++) {
            if (temp[j].frequency > temp[i].frequency) {
                Suggestion t = temp[i];
                temp[i] = temp[j];
                temp[j] = t;
            }
        }
    }

    // Copy top 'limit' results
    count = (tempCount < limit) ? tempCount : limit;
    for (int i = 0; i < count; i++) {
        results[i] = temp[i];
    }

    return count;
}
