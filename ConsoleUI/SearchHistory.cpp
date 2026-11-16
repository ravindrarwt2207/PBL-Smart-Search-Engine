
#include "SearchHistory.h"

#include <ctime>
#include <exception>
#include <fstream>

SearchHistory::SearchHistory(const std::string& fileName) : fileName_(fileName) {}

std::string SearchHistory::currentTimestamp() {
    std::time_t now = std::time(nullptr);
    std::tm     localTime{};
#ifdef _WIN32
    localtime_s(&localTime, &now);
#else
    localtime_r(&now, &localTime);
#endif
    char buffer[32];
    if (std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &localTime) == 0) {
        return "unknown time";
    }
    return buffer;
}

std::string SearchHistory::cleanText(const std::string& text) {
    std::string result = text;
    for (char& c : result) {
        if (c == '\n' || c == '\r') c = ' ';
    }
    return result;
}

bool SearchHistory::load() {
    entries_.clear();

    std::ifstream file(fileName_);
    if (!file.is_open()) {
        return false; 
    }

    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back(); 

        std::size_t firstBar = line.find('|');
        if (firstBar == std::string::npos) continue;
        std::size_t secondBar = line.find('|', firstBar + 1);
        if (secondBar == std::string::npos) continue;

        HistoryEntry entry;
        entry.timestamp = line.substr(0, firstBar);
        entry.query     = line.substr(secondBar + 1);
        try {
            entry.resultCount =
                std::stoi(line.substr(firstBar + 1, secondBar - firstBar - 1));
        } catch (const std::exception&) {
            continue;  
        }
        if (entry.query.empty()) continue;
        entries_.push_back(entry);
    }
    return true;
}

bool SearchHistory::save() {
    std::ofstream file(fileName_, std::ios::trunc);
    if (!file.is_open()) {
        return false;
    }
    for (const HistoryEntry& entry : entries_) {
        file << entry.timestamp << '|' << entry.resultCount << '|' << entry.query << '\n';
    }
    return file.good();
}

void SearchHistory::addEntry(const std::string& query, int resultCount) {
    HistoryEntry entry;
    entry.timestamp   = currentTimestamp();
    entry.query       = cleanText(query);
    entry.resultCount = resultCount;
    entries_.push_back(entry);
}

const std::vector<HistoryEntry>& SearchHistory::getEntries() const { return entries_; }
std::size_t SearchHistory::size() const { return entries_.size(); }
bool SearchHistory::isEmpty() const { return entries_.empty(); }
void SearchHistory::clear() { entries_.clear(); }
