#ifndef TRIE_H
#define TRIE_H

#include <string>
#include <unordered_map>
#include <vector>

class Trie {

private:

    struct TrieNode {

        std::unordered_map<char, TrieNode*> children;

        bool isEndOfWord;
        int frequency;

        TrieNode() {
            isEndOfWord = false;
            frequency = 0;
        }
    };

    TrieNode* root;

    void collectWords(
        TrieNode* node,
        std::string currentWord,
        std::vector<std::pair<std::string, int>>& results
    );

    void deleteTrie(TrieNode* node);

public:

    Trie();

    ~Trie();

    // Prevent unsafe copying of raw-pointer-owned Trie
    Trie(const Trie&) = delete;
    Trie& operator=(const Trie&) = delete;

    void insert(const std::string& word);

    std::vector<std::string> getSuggestions(
        const std::string& prefix,
        int limit = 5
    );

    int getFrequency(const std::string& word);

};

#endif