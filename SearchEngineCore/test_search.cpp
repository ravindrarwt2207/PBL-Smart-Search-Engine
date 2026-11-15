#include <iostream>
#include <string>
#include <vector>
#include "searchengine.h"

int main() {
    SearchEngine engine;

    std::vector<Document> documents = {
        {1, "Programming Notes",
         "C++ programming uses data structures."},

        {2, "Data Structures",
         "Data structures organize data."},

        {3, "Autocomplete",
         "A trie supports autocomplete and prefix search."}
    };

    for (const Document& doc : documents) {
        engine.indexDocument(doc);
    }

    std::cout << "SMART SEARCH - CORE DEMO\n";
    std::cout << "Type /exit to quit.\n";

    std::string query;

    while (true) {
        std::cout << "\nEnter query: ";

        if (!std::getline(std::cin, query) ||
            query == "/exit") {
            break;
        }

        const auto results = engine.search(query, 10);

        if (results.empty()) {
            std::cout << "No matching documents.\n";
            continue;
        }

        for (const SearchResult& result : results) {
            for (const Document& doc : documents) {
                if (doc.id == result.documentId) {
                    std::cout << "\nID: " << doc.id
                              << "\nTitle: " << doc.title
                              << "\nScore: " << result.score
                              << "\nPreview: "
                              << doc.content.substr(0, 100)
                              << '\n';
                    break;
                }
            }
        }
    }

    return 0;
}
