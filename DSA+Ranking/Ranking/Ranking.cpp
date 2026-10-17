#include "Ranking.h"
#include <cctype>
#include <algorithm>

RankingEngine::RankingEngine() {
}

double RankingEngine::calculateTFScore(const Candidate& doc) {
    if (doc.docLength <= 0) return 0.0;

    double tf = (double)doc.termFrequency / doc.docLength;
    return std::min(tf * 10.0, 1.0);   // scale between 0 to 1
}

double RankingEngine::calculateTitleMatchBonus(const Candidate& doc, const std::string& query) {
    if (query.empty() || doc.title.empty()) return 0.0;

    std::string lowerTitle = doc.title;
    std::string lowerQuery = query;

    for (char &c : lowerTitle) c = std::tolower((unsigned char)c);
    for (char &c : lowerQuery) c = std::tolower((unsigned char)c);

    if (lowerTitle == lowerQuery) return 1.0;

    if (lowerTitle.find(lowerQuery) != std::string::npos) return 0.7;

    return 0.2;
}

int RankingEngine::rankDocuments(const Candidate candidates[], int count,
                                 const std::string& query,
                                 RankedResult results[], int topK) {

    if (count <= 0 || topK <= 0) return 0;

    MaxHeap heap;

    for (int i = 0; i < count; i++) {
        const Candidate& doc = candidates[i];

        double tfScore = calculateTFScore(doc);
        double titleScore = calculateTitleMatchBonus(doc, query);


        double finalScore = (0.6 * tfScore) + (0.4 * titleScore);

        std::string snippet = doc.content.substr(0, 80);
        if (doc.content.length() > 80) {
            snippet += "...";
        }

        RankedResult ranked(doc.docID, doc.title, snippet, doc.category, finalScore);
        heap.push(ranked);
    }
    int resultCount = 0;
    while (!heap.empty() && resultCount < topK) {
        results[resultCount] = heap.top();
        heap.pop();
        resultCount++;
    }

    return resultCount;
}