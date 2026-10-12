#ifndef TRIE_H
#define TRIE_H

#include <string>
#include <vector>
#include <unordered_map>
#include "../Common/Common.h"

class TrieNode {
public:
    std::unordered_map<char, TrieNode*> children;
    bool isEndOfWord;
    int frequency;
    std::string word;

    TrieNode() {
        isEndOfWord = false;
        frequency = 0;
    }
};

class Trie {
private:
    TrieNode* root;

    void collectSuggestions(TrieNode* node, std::vector<Suggestion>& results, int limit);
    void clearNode(TrieNode* node);

public:
    Trie();
    \~Trie();

    void insert(const std::string& word, int freq = 1);
    std::vector<Suggestion> getSuggestions(const std::string& prefix, int limit = 5);
    bool search(const std::string& word);
    bool startsWith(const std::string& prefix);
    void clear();
};

#endif
