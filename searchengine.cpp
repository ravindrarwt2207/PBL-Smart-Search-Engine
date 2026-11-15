#include "searchengine.h"
#include "TextProcessor.h"
#include <iostream>
#include <queue>
#include <unordered_set>

void SearchEngine::indexDocument(const Document& doc) {
    // Remove existing entries for this ID to prevent double counting.
    for (auto it = invertedIndex.begin();
         it != invertedIndex.end();) {

        it->second.erase(doc.id);

        if (it->second.empty()) {
            it = invertedIndex.erase(it);
        } else {
            ++it;
        }
    }

    std::vector<std::string> words =
        TextProcessor::tokenize(doc.content);

    for (const std::string& word : words) {
        ++invertedIndex[word][doc.id];
    }
}

void SearchEngine::displayIndex() const {
    for (const auto& entry : invertedIndex) {
        std::cout << entry.first << " -> ";

        for (const auto& posting : entry.second) {
            std::cout << "[Document " << posting.first
                      << ": " << posting.second << "] ";
        }

        std::cout << '\n';
    }
}
std::vector<SearchResult> SearchEngine::search(
    const std::string& query,
    int limit
) const {
    if (limit <= 0) {
        return {};
    }

    std::vector<std::string> words =
        TextProcessor::tokenize(query);

    // Repeated query words should count only once.
    std::unordered_set<std::string> uniqueWords(
        words.begin(), words.end()
    );

    std::unordered_map<int, double> scores;

    for (const std::string& word : uniqueWords) {
        auto found = invertedIndex.find(word);

        if (found == invertedIndex.end()) {
            continue;
        }

        for (const auto& posting : found->second) {
            int documentId = posting.first;
            int frequency = posting.second;

            scores[documentId] += frequency + 5.0;
        }
    }

    // Higher scores come first.
    // For equal scores, smaller document IDs come first.
    auto compare = [](const SearchResult& a,
                      const SearchResult& b) {
        if (a.score != b.score) {
            return a.score < b.score;
        }

        return a.documentId > b.documentId;
    };

    std::priority_queue<
        SearchResult,
        std::vector<SearchResult>,
        decltype(compare)
    > rankedResults(compare);

    for (const auto& entry : scores) {
        rankedResults.push({entry.first, entry.second});
    }

    std::vector<SearchResult> results;

    while (!rankedResults.empty() &&
           results.size() < static_cast<std::size_t>(limit)) {
        results.push_back(rankedResults.top());
        rankedResults.pop();
    }

    return results;
}
