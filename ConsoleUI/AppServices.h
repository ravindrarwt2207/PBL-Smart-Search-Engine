
#ifndef APP_SERVICES_H
#define APP_SERVICES_H

#include <cstddef>
#include <string>
#include <vector>

namespace AppServices {

struct DocumentInfo {
    int         id;
    std::string title;
};

struct Document {
    int         id;
    std::string title;
    std::string content;
};

struct SearchResult {
    int         docId;
    std::string title;
    double      score;    
    std::string preview;   
                           
};
bool importDocument(const std::string& filePath, std::string& message);
int processPendingIndexing();

std::vector<DocumentInfo> listDocuments();
bool getDocumentById(int id, Document& outDocument);
std::vector<SearchResult> searchDocuments(const std::string& query);

std::vector<std::string> getAutocompleteSuggestions(const std::string& prefix,
                                                    std::size_t maxSuggestions = 5);

}  

#endif
