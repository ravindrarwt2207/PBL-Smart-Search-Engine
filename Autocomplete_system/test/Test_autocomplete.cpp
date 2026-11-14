#include <iostream>
#include <cassert>
#include <vector>
#include <string>

#include "Trie.h"
#include "SuggestionEngine.h"
#include "Utils.h"

using namespace std;

void testInsertionAndFrequency() {

    Trie trie;

    trie.insert("program");
    trie.insert("program");
    trie.insert("programming");

    assert(trie.getFrequency("program") == 2);
    assert(trie.getFrequency("programming") == 1);
    assert(trie.getFrequency("unknown") == 0);

    cout << "Insertion and frequency test passed!\n";
}

void testPrefixSearch() {

    Trie trie;

    trie.insert("program");
    trie.insert("programming");
    trie.insert("project");

    vector<string> result =
        trie.getSuggestions("prog", 5);

    assert(result.size() == 2);

    cout << "Prefix search test passed!\n";
}

void testFrequencyRanking() {

    Trie trie;

    trie.insert("apple");
    trie.insert("apple");
    trie.insert("application");
    trie.insert("application");
    trie.insert("application");
    trie.insert("apply");

    vector<string> result =
        trie.getSuggestions("app", 5);

    assert(result.size() == 3);
    assert(result[0] == "application");
    assert(result[1] == "apple");
    assert(result[2] == "apply");

    cout << "Frequency ranking test passed!\n";
}

void testTopFive() {

    Trie trie;

    trie.insert("apple");
    trie.insert("application");
    trie.insert("apply");
    trie.insert("appetite");
    trie.insert("apparel");
    trie.insert("append");

    vector<string> result =
        trie.getSuggestions("app", 5);

    assert(result.size() == 5);

    cout << "Top five test passed!\n";
}

void testUnknownPrefix() {

    Trie trie;

    trie.insert("computer");

    vector<string> result =
        trie.getSuggestions("xyz", 5);

    assert(result.empty());

    cout << "Unknown prefix test passed!\n";
}

void testNormalization() {

    vector<string> words = {
        "PROGRAMMING",
        "Computer!",
        "Algorithm"
    };

    vector<string> result =
        Utils::normalizeWords(words);

    assert(result[0] == "programming");
    assert(result[1] == "computer");
    assert(result[2] == "algorithm");

    cout << "Normalization test passed!\n";
}

void testDynamicUpdate() {

    SuggestionEngine engine;

    engine.addWords({"program", "project"});

    engine.updateVocabulary({"programming"});

    vector<string> result =
        engine.suggest("prog", 5);

    assert(result.size() == 2);

    cout << "Dynamic update test passed!\n";
}

int main() {

    cout << "\n===== AUTOCOMPLETE TESTING =====\n\n";

    testInsertionAndFrequency();

    testPrefixSearch();

    testFrequencyRanking();

    testTopFive();

    testUnknownPrefix();

    testNormalization();

    testDynamicUpdate();

    cout << "\nAll tests passed successfully!\n";

    return 0;
}