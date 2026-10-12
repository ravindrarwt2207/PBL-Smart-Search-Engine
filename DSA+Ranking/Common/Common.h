#ifndef COMMON_H
#define COMMON_H

#include <string>
#include <vector>
#include <ctime>

// ==================== Candidate Document (Input from Search Module) ====================
struct Candidate {
    int docID;
    std::string title;
    std::string content;          // or snippet
    std::string category;         // e.g. "Notice", "Faculty", "Event", "Subject"
    int termFrequency;            // how many times query terms appear in document
    int docLength;                // total words in the document
    long long timestamp;          // when document was added/updated (unix time)
    int popularity;               // number of times this document was clicked/viewed

    // Default constructor
    Candidate() : docID(0), termFrequency(0), docLength(1), timestamp(0), popularity(0) {}
};

// ==================== Ranked Result (Your Output) ====================
struct RankedResult {
    int docID;
    std::string title;
    std::string snippet;
    std::string category;
    double score;

    // Default constructor
    RankedResult() : docID(0), score(0.0) {}

    // Parameterized constructor
    RankedResult(int id, const std::string& t, const std::string& s, 
                 const std::string& cat, double sc)
        : docID(id), title(t), snippet(s), category(cat), score(sc) {}
};

// ==================== Suggestion Item ====================
struct Suggestion {
    std::string text;
    int frequency;                // how popular this suggestion is

    // Default constructor
    Suggestion() : frequency(0) {}

    // Parameterized constructor
    Suggestion(const std::string& t, int f) : text(t), frequency(f) {}
};

#endif // COMMON_H
