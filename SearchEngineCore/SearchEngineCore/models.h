#ifndef MODELS_H
#define MODELS_H

#include <string>

struct Document {
    int id;
    std::string title;
    std::string content;
};

struct SearchResult {
    int documentId;
    double score;
};

#endif
