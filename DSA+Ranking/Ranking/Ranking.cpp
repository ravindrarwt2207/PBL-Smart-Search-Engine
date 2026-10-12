#include "Ranking.h"
#include <cmath>
#include <cctype>
#include <ctime>
#include <algorithm>

RankingEngine::RankingEngine() {
    weightTF = 0.45;
    weightTitle = 0.25;
    weightCategory = 0.10;
    weightRecency = 0.10;
    weightPopularity = 0.10;
}

void RankingEngine::setWeights(double tf, double title, double category, 
                               double recency, double popularity) {
    weightTF = tf;
    weightTitle = title;
    weightCategory = category;
    weightRecency = recency;
    weightPopularity = popularity;
}

double RankingEngine::calculateTFScore(const Candidate& doc) {
    if (doc.docLength <= 0) return 0.0;
    double tf = static_cast<double>(doc.termFrequency) / doc.docLength;
    return std::min(tf * 10.0, 1.0);
}

double RankingEngine::calculateTitleMatchBonus(const Candidate& doc, const std::string& query) {
    if (query.empty() || doc.title.empty()) return 0.0;

    std::string lowerTitle = doc.title;
    std::string lowerQuery = query;

    for (char& c : lowerTitle) c = std::tolower(static_cast<unsigned char>(c));
    for (char& c : lowerQuery) c = std::tolower(static_cast<unsigned char>(c));

    if (lowerTitle == lowerQuery) return 1.0;
    if (lowerTitle.find(lowerQuery) != std::string::npos) return 0.8;

    return 0.3;
}

double RankingEngine::calculateCategoryScore(const Candidate& doc, const std::string& query) {
    std::string lowerCat = doc.category;
    std::string lowerQuery = query;

    for (char& c : lowerCat) c = std::tolower(static_cast<unsigned char>(c));
    for (char& c : lowerQuery) c = std::tolower(static_cast<unsigned char>(c));

    if (lowerQuery.find(lowerCat) != std::string::npos || 
        lowerCat.find(lowerQuery) != std::string::npos) {
        return 0.7;
    }
    return 0.2;
}

double RankingEngine::calculateRecencyScore(const Candidate& doc) {
    if (doc.timestamp <= 0) return 0.3;

    time_t now = time(nullptr);
    double daysOld = difftime(now, doc.timestamp) / (60.0 * 60.0 * 24.0);

    if (daysOld <= 1) return 1.0;
    if (daysOld <= 7) return 0.8;
    if (daysOld <= 30) return 0.5;
    if (daysOld <= 90) return 0.3;
    return 0.1;
}

double RankingEngine::calculatePopularityScore(const Candidate& doc) {
    if (doc.popularity <= 0) return 0.1;
    return std::min(std::log10(doc.popularity + 1) / 3.0, 1.0);
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
        double catScore = calculateCategoryScore(doc, query);
        double recencyScore = calculateRecencyScore(doc);
        double popScore = calculatePopularityScore(doc);

        double finalScore = weightTF * tfScore
                          + weightTitle * titleScore
                          + weightCategory * catScore
                          + weightRecency * recencyScore
                          + weightPopularity * popScore;

        // Create short snippet
        std::string snippet = doc.content.substr(0, 100);
        if (doc.content.length() > 100) {
            snippet += "...";
        }

        RankedResult ranked(doc.docID, doc.title, snippet, doc.category, finalScore);
        heap.push(ranked);
    }

    // Extract topK results
    int resultCount = 0;
    while (!heap.empty() && resultCount < topK) {
        results[resultCount] = heap.top();
        heap.pop();
        resultCount++;
    }

    return resultCount;
}
