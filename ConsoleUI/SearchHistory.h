// ============================================================================
// SearchHistory.h  -  Stores, saves and loads previous searches (Aszad)
// ============================================================================
#ifndef SEARCH_HISTORY_H
#define SEARCH_HISTORY_H

#include <cstddef>
#include <string>
#include <vector>

// One saved search.
struct HistoryEntry {
    std::string timestamp;     // e.g. "2026-11-15 16:50:12"
    std::string query;         // what the user searched for
    int         resultCount;   // how many results were found
};

class SearchHistory {
public:
    // fileName: the text file used to keep history between runs.
    explicit SearchHistory(const std::string& fileName = "search_history.txt");

    bool load();   // read the file (called when the program starts)
    bool save();   // write the file (called when the program exits)

    // Adds a search using the CURRENT date and time.
    void addEntry(const std::string& query, int resultCount);

    const std::vector<HistoryEntry>& getEntries() const;
    std::size_t size() const;
    bool        isEmpty() const;
    void        clear();

private:
    static std::string currentTimestamp();
    static std::string cleanText(const std::string& text);

    std::string               fileName_;
    std::vector<HistoryEntry> entries_;
};

#endif  // SEARCH_HISTORY_H
