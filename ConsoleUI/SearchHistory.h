#ifndef SEARCH_HISTORY_H
#define SEARCH_HISTORY_H

#include <cstddef>
#include <string>
#include <vector>

struct HistoryEntry {
    std::string timestamp;    
    std::string query;        
    int         resultCount;  
};

class SearchHistory {
public:
 
    explicit SearchHistory(const std::string& fileName = "search_history.txt");

    bool load();  
    bool save();   

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

#endif  
