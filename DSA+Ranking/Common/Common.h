#ifndef COMMON_H
#define COMMON_H

#include <string>
#include <ctime>

// ==================== Candidate Document ====================
struct Candidate {
    int docID;
    std::string title;
    std::string content;
    std::string category;
    int termFrequency;
    int docLength;
    long long timestamp;
    int popularity;

    Candidate() : docID(0), termFrequency(0), docLength(1), timestamp(0), popularity(0) {}
};

// ==================== Ranked Result ====================
struct RankedResult {
    int docID;
    std::string title;
    std::string snippet;
    std::string category;
    double score;

    RankedResult() : docID(0), score(0.0) {}

    RankedResult(int id, const std::string& t, const std::string& s, 
                 const std::string& cat, double sc)
        : docID(id), title(t), snippet(s), category(cat), score(sc) {}
};

// ==================== Suggestion ====================
struct Suggestion {
    std::string text;
    int frequency;

    Suggestion() : frequency(0) {}
    Suggestion(const std::string& t, int f) : text(t), frequency(f) {}
};

#endif
