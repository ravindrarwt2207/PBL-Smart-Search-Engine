#ifndef RANKING_H
#define RANKING_H

#include <string>
#include "../Common/Common.h"
#include "../Heap/MaxHeap.h"

class RankingEngine {
private:
    double calculateTFScore(const Candidate& doc);
    double calculateTitleMatchBonus(const Candidate& doc, const std::string& query);

public:
    RankingEngine();

    int rankDocuments(const Candidate candidates[], int count,
                      const std::string& query,
                      RankedResult results[], int topK = 10);
};

#endif
