#include <iostream>
#include <string>
#include <ctime>
#include "SuggestionEngine/SuggestionEngine.h"
#include "Ranking/Ranking.h"
#include "Common/Common.h"

using namespace std;

int main() {
    cout << "=============================================\n";
    cout << "   DSA + Search Ranking Module - Test Demo  \n";
    cout << "=============================================\n\n";

    // =====================================================
    // PART 1: Testing Search Suggestions (Trie)
    // =====================================================
    cout << ">>> PART 1: Testing Search Suggestions\n";
    cout << "---------------------------------------\n";

    SuggestionEngine suggestionEngine;

    // Adding sample suggestions
    suggestionEngine.addSuggestion("Computer Networks", 25);
    suggestionEngine.addSuggestion("Computer Organization", 18);
    suggestionEngine.addSuggestion("Data Structures", 40);
    suggestionEngine.addSuggestion("Database Management System", 30);
    suggestionEngine.addSuggestion("Operating System", 22);
    suggestionEngine.addSuggestion("Object Oriented Programming", 28);
    suggestionEngine.addSuggestion("Artificial Intelligence", 15);
    suggestionEngine.addSuggestion("Machine Learning", 20);
    suggestionEngine.addSuggestion("Web Development", 12);
    suggestionEngine.addSuggestion("Compiler Design", 10);

    // Test suggestions
    string testPrefixes[] = {"comp", "data", "op", "ma"};
    int numPrefixes = 4;

    for (int i = 0; i < numPrefixes; i++) {
        cout << "\nSuggestions for \"" << testPrefixes[i] << "\":\n";

        Suggestion results[5];
        int found = suggestionEngine.getSuggestions(testPrefixes[i], results, 5);

        if (found == 0) {
            cout << "  No suggestions found.\n";
        } else {
            for (int j = 0; j < found; j++) {
                cout << "  " << (j + 1) << ". " << results[j].text 
                     << " (freq: " << results[j].frequency << ")\n";
            }
        }
    }

    // =====================================================
    // PART 2: Testing Ranking Engine
    // =====================================================
    cout << "\n\n>>> PART 2: Testing Ranking Engine\n";
    cout << "----------------------------------\n";

    RankingEngine rankingEngine;

    // Creating sample candidate documents
    Candidate candidates[5];

    // Document 1
    candidates[0].docID = 101;
    candidates[0].title = "Introduction to Data Structures";
    candidates[0].content = "This document explains arrays, linked lists, stacks, queues and trees.";
    candidates[0].category = "Subject";
    candidates[0].termFrequency = 8;
    candidates[0].docLength = 120;
    candidates[0].timestamp = time(nullptr) - 2 * 24 * 3600;
    candidates[0].popularity = 150;

    // Document 2
    candidates[1].docID = 102;
    candidates[1].title = "Data Structures and Algorithms Notes";
    candidates[1].content = "Complete notes on DSA including sorting, searching and graphs.";
    candidates[1].category = "Notes";
    candidates[1].termFrequency = 12;
    candidates[1].docLength = 200;
    candidates[1].timestamp = time(nullptr) - 10 * 24 * 3600;
    candidates[1].popularity = 320;

    // Document 3
    candidates[2].docID = 103;
    candidates[2].title = "Operating System Concepts";
    candidates[2].content = "Process management, memory management and file systems.";
    candidates[2].category = "Subject";
    candidates[2].termFrequency = 3;
    candidates[2].docLength = 180;
    candidates[2].timestamp = time(nullptr) - 1 * 24 * 3600;
    candidates[2].popularity = 90;

    // Document 4
    candidates[3].docID = 104;
    candidates[3].title = "Advanced Data Structures";
    candidates[3].content = "Trees, heaps, segment trees and advanced graph algorithms.";
    candidates[3].category = "Subject";
    candidates[3].termFrequency = 15;
    candidates[3].docLength = 90;
    candidates[3].timestamp = time(nullptr) - 5 * 24 * 3600;
    candidates[3].popularity = 210;

    // Document 5
    candidates[4].docID = 105;
    candidates[4].title = "Database Systems";
    candidates[4].content = "Relational databases, SQL, normalization and transactions.";
    candidates[4].category = "Subject";
    candidates[4].termFrequency = 2;
    candidates[4].docLength = 150;
    candidates[4].timestamp = time(nullptr) - 40 * 24 * 3600;
    candidates[4].popularity = 75;

    // Rank documents
    string query = "data structures";
    cout << "\nRanking results for query: \"" << query << "\"\n\n";

    RankedResult rankedResults[5];
    int resultCount = rankingEngine.rankDocuments(candidates, 5, query, rankedResults, 5);

    for (int i = 0; i < resultCount; i++) {
        cout << (i + 1) << ". [Score: " << rankedResults[i].score << "] "
             << rankedResults[i].title << " (" << rankedResults[i].category << ")\n";
        cout << "   " << rankedResults[i].snippet << "\n\n";
    }

    cout << "=============================================\n";
    cout << "           Testing Completed Successfully    \n";
    cout << "=============================================\n";

    return 0;
}
