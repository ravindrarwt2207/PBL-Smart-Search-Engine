// ============================================================================
// AppServices.h  -  SHARED INTERFACE between all team members' modules
// ----------------------------------------------------------------------------
// The Console UI (Aszad) only talks to the other modules through these
// functions. It never includes DocumentManager, SearchEngine or Autocomplete
// headers directly.
//
// Who implements what (in a file called AppServices.cpp, written by the team):
//   importDocument / processPendingIndexing / listDocuments / getDocumentById
//        -> forward to Piyush's DocumentManager
//   searchDocuments
//        -> forward to Dewang's SearchEngine
//   getAutocompleteSuggestions
//        -> forward to Ravinder's Autocomplete
//
// Each function below is only DECLARED here. Exactly one .cpp file in the
// project must DEFINE them, otherwise the linker will report "undefined
// reference".
// ============================================================================
#ifndef APP_SERVICES_H
#define APP_SERVICES_H

#include <cstddef>
#include <string>
#include <vector>

namespace AppServices {

// Short information about one stored document (used in the document list).
struct DocumentInfo {
    int         id;
    std::string title;
};

// A complete document (used when the user opens a search result).
struct Document {
    int         id;
    std::string title;
    std::string content;
};

// One ranked search result. Results must be returned best match first.
struct SearchResult {
    int         docId;
    std::string title;
    double      score;     // relevance score (higher = better)
    std::string preview;   // short content preview; may be left empty,
                           // the UI will then build one from the document text
};

// 1. Import a .txt file. Return true on success.
//    'message' receives a human-readable explanation (success or error).
bool importDocument(const std::string& filePath, std::string& message);

// 2. Index every document that is waiting in the indexing queue.
//    Returns how many documents were indexed.
int processPendingIndexing();

// 3. All documents currently stored.
std::vector<DocumentInfo> listDocuments();

// 4. Fetch one document by its ID. Return false if the ID does not exist.
bool getDocumentById(int id, Document& outDocument);

// 5. Run a search (single or multi-word query). Ranked, best first.
std::vector<SearchResult> searchDocuments(const std::string& query);

// 6. Autocomplete suggestions for the word currently being typed.
//    Returns at most 'maxSuggestions' words.
std::vector<std::string> getAutocompleteSuggestions(const std::string& prefix,
                                                    std::size_t maxSuggestions = 5);

}  // namespace AppServices

#endif  // APP_SERVICES_H
