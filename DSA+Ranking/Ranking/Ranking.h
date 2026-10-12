#ifndef RANKING_H
#define RANKING_H

#include <string>
#include "../Common/Common.h"
#include "../Heap/MaxHeap.h"

class RankingEngine {
private:
    double weightTF;
    double weightTitle;
    double weightCategory;
    double weightRecency;
    double weightPopularity;

    double calculateTFScore(const Candidate& doc);
    double calculateTitleMatchBonus(const Candidate& doc, const std::string& query);
    double calculateCategoryScore(const Candidate& doc, const std::string& query);
    double calculateRecencyScore(const Candidate& doc);
    double calculatePopularityScore(const Candidate& doc);

public:
    RankingEngine();

    // Main ranking function
    // candidates[] = list of documents from Search module
    // count = how many candidates
    // results[] = output ranked results
    // topK = how many top results you want
    // Returns the number of results filled
    int rankDocuments(const Candidate candidates[], int count, 
                      const std::string& query, 
                      RankedResult results[], int topK = 10);

    void setWeights(double tf, double title, double category, double recency, double popularity);
};

#endif
