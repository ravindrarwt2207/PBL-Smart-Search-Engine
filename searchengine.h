#ifndef SEARCH_ENGINE_H
#define SEARCH_ENGINE_H

#include "models.h"
#include <string>
#include <vector>
#include <unordered_map>

class SearchEngine {
private:
    std::unordered_map<
        std::string,
        std::unordered_map<int, int>
    > invertedIndex;

public:
std::vector<SearchResult> search(
    const std::string& query,
    int limit = 10
) const;
    void indexDocument(const Document& doc);
    void displayIndex() const;
};

#endif