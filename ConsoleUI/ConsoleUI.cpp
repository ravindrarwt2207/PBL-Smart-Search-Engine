
#include "ConsoleUI.h"

#include <algorithm>
#include <cctype>
#include <exception>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace {
const int kLineWidth = 60;
}
ConsoleUI::ConsoleUI() : history_("search_history.txt"), inputEnded_(false), running_(true) {}

void ConsoleUI::run() {
    history_.load();
    while (running_) {
        try {
            showHeader();
            showMainMenu();

            int choice = readInt("Enter your choice: ", 1, 6);
            if (choice == -1) {
                exitScreen();
                break;
            }

            switch (choice) {
                case 1: addDocumentScreen();       break;
                case 2: viewAllDocumentsScreen();  break;
                case 3: searchScreen();            break;
                case 4: historyScreen();           break;
                case 5: projectInfoScreen();       break;
                case 6: exitScreen();              break;
            }
        } catch (const std::exception& error) {
            std::cout << "\n[ERROR] Something went wrong: " << error.what() << "\n";
            waitForEnter();
        } catch (...) {
            std::cout << "\n[ERROR] An unknown error occurred.\n";
            waitForEnter();
        }
    }
}

void ConsoleUI::printLine(char symbol, int width) {
    std::cout << std::string(static_cast<std::size_t>(width), symbol) << "\n";
}

void ConsoleUI::printTitle(const std::string& title) {
    std::cout << "\n";
    printLine('-', kLineWidth);
    std::cout << "  " << title << "\n";
    printLine('-', kLineWidth);
}

void ConsoleUI::showHeader() const {
    std::cout << "\n";
    printLine('=', kLineWidth);
    std::cout << "        SMART AUTOCOMPLETE & SEARCH ENGINE\n\n";
    std::cout << "              Developed by: team cursor\n";
    printLine('=', kLineWidth);
}

void ConsoleUI::showMainMenu() const {
    std::cout << "\n"
              << "  1. Add New Document\n"
              << "  2. View All Documents\n"
              << "  3. Search Documents\n"
              << "  4. View Search History\n"
              << "  5. Project Information\n"
              << "  6. Exit\n\n";
}
bool ConsoleUI::readLine(std::string& line) {
    if (!std::getline(std::cin, line)) {
        inputEnded_ = true;
        line.clear();
        return false;
    }
    return true;
}

int ConsoleUI::readInt(const std::string& prompt, int min, int max) {
    while (true) {
        std::cout << prompt;
        std::string line;
        if (!readLine(line)) return -1;
        line = trim(line);

        if (line.empty()) {
            std::cout << "  Please enter a number between " << min << " and " << max << ".\n";
            continue;
        }
        bool allDigits = std::all_of(line.begin(), line.end(), [](unsigned char c) {
            return std::isdigit(c) != 0;
        });
        if (!allDigits || line.size() > 6) {
            std::cout << "  Invalid input. Please enter a number between " << min << " and "
                      << max << ".\n";
            continue;
        }
        int value = std::stoi(line);
        if (value < min || value > max) {
            std::cout << "  Out of range. Please enter a number between " << min << " and "
                      << max << ".\n";
            continue;
        }
        return value;
    }
}

void ConsoleUI::waitForEnter() {
    if (inputEnded_) return;
    std::cout << "\nPress Enter to continue...";
    std::string ignored;
    readLine(ignored);
}

std::string ConsoleUI::trim(const std::string& text) {
    std::size_t start = 0;
    while (start < text.size() && std::isspace(static_cast<unsigned char>(text[start]))) ++start;
    std::size_t end = text.size();
    while (end > start && std::isspace(static_cast<unsigned char>(text[end - 1]))) --end;
    return text.substr(start, end - start);
}

std::string ConsoleUI::stripQuotes(const std::string& text) {
    std::string result = trim(text);
    if (result.size() >= 2 && ((result.front() == '"' && result.back() == '"') ||
                               (result.front() == '\'' && result.back() == '\''))) {
        result = trim(result.substr(1, result.size() - 2));
    }
    return result;
}

std::string ConsoleUI::toLower(const std::string& text) {
    std::string result = text;
    for (char& c : result) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    return result;
}

bool ConsoleUI::hasTxtExtension(const std::string& path) {
    std::string lower = toLower(path);
    return lower.size() > 4 && lower.compare(lower.size() - 4, 4, ".txt") == 0;
}

std::string ConsoleUI::shorten(const std::string& text, std::size_t maxLength) {
    if (text.size() <= maxLength) return text;
    if (maxLength <= 3) return text.substr(0, maxLength);
    return text.substr(0, maxLength - 3) + "...";
}

std::string ConsoleUI::makePreview(const std::string& text, std::size_t maxLength) {
    std::string flat;
    bool lastWasSpace = true;  // also removes leading spaces
    for (char c : text) {
        if (std::isspace(static_cast<unsigned char>(c))) {
            if (!lastWasSpace) flat += ' ';
            lastWasSpace = true;
        } else {
            flat += c;
            lastWasSpace = false;
        }
    }
    flat = trim(flat);
    return shorten(flat, maxLength);
}

std::string ConsoleUI::lastWord(const std::string& query) {
    std::string q = trim(query);
    std::size_t pos = q.find_last_of(" \t");
    return (pos == std::string::npos) ? q : q.substr(pos + 1);
}

std::string ConsoleUI::replaceLastWord(const std::string& query, const std::string& word) {
    std::string q = trim(query);
    std::size_t pos = q.find_last_of(" \t");
    if (pos == std::string::npos) return word;
    return q.substr(0, pos + 1) + word;
}
void ConsoleUI::addDocumentScreen() {
    printTitle("ADD NEW DOCUMENT");
    std::cout << "Enter the full path of a .txt file (or press Enter to go back).\n"
              << "Example: C:\\docs\\notes.txt   or   /home/user/notes.txt\n\n"
              << "File path: ";

    std::string input;
    if (!readLine(input)) return;
    std::string path = stripQuotes(input);

    if (path.empty()) {
        std::cout << "\n  No file entered. Returning to the main menu.\n";
        waitForEnter();
        return;
    }
    if (!hasTxtExtension(path)) {
        std::cout << "\n  [FAILED] Only .txt files are supported.\n";
        waitForEnter();
        return;
    }

    std::string message;
    bool imported = AppServices::importDocument(path, message);

    if (!imported) {
        std::cout << "\n  [FAILED] Document could not be imported.\n";
        if (!message.empty()) std::cout << "  Reason: " << message << "\n";
        waitForEnter();
        return;
    }

    std::cout << "\n  [SUCCESS] Document imported successfully.\n";
    if (!message.empty()) std::cout << "  " << message << "\n";

    // Automatically process the indexing queue.
    std::cout << "  Processing indexing queue...\n";
    int indexed = AppServices::processPendingIndexing();
    std::cout << "  Indexing complete. Documents indexed: " << indexed << "\n";
    waitForEnter();
}

void ConsoleUI::viewAllDocumentsScreen() {
    printTitle("ALL DOCUMENTS");

    std::vector<AppServices::DocumentInfo> docs = AppServices::listDocuments();
    if (docs.empty()) {
        std::cout << "  No documents found. Use option 1 to add a document.\n";
        waitForEnter();
        return;
    }

    std::cout << "  " << std::left << std::setw(8) << "ID" << "Title\n";
    std::cout << "  ";
    printLine('-', kLineWidth - 2);
    for (const AppServices::DocumentInfo& doc : docs) {
        std::cout << "  " << std::left << std::setw(8) << doc.id << shorten(doc.title, 48)
                  << "\n";
    }
    std::cout << "\n  Total documents: " << docs.size() << "\n";
    waitForEnter();
}

std::string ConsoleUI::askForQueryWithAutocomplete() {
    std::cout << "Enter your search query (single or multiple words).\n"
              << "Search: ";
    std::string input;
    if (!readLine(input)) return "";
    std::string query = trim(input);
    if (query.empty()) return "";

    std::string current = lastWord(query);
    std::vector<std::string> suggestions = AppServices::getAutocompleteSuggestions(current, 5);
    if (suggestions.size() > 5) suggestions.resize(5);

    if (!suggestions.empty()) {
        std::cout << "\n  Autocomplete suggestions for \"" << current << "\":\n";
        for (std::size_t i = 0; i < suggestions.size(); ++i) {
            std::cout << "    " << (i + 1) << ". " << suggestions[i] << "\n";
        }
        std::cout << "\n  Type a suggestion number to use it, or press Enter to keep your query: ";
        std::string answer;
        if (!readLine(answer)) return query;
        answer = trim(answer);

        if (!answer.empty()) {
            bool isNumber = std::all_of(answer.begin(), answer.end(), [](unsigned char c) {
                return std::isdigit(c) != 0;
            });
            if (isNumber && answer.size() <= 3) {
                int pick = std::stoi(answer);
                if (pick >= 1 && pick <= static_cast<int>(suggestions.size())) {
                    query = replaceLastWord(query, suggestions[static_cast<std::size_t>(pick - 1)]);
                    std::cout << "  Using query: " << query << "\n";
                } else {
                    std::cout << "  Not a valid suggestion number. Keeping your query.\n";
                }
            } else {
                std::cout << "  Keeping your query.\n";
            }
        }
    } else {
        std::cout << "\n  (No autocomplete suggestions for \"" << current << "\")\n";
    }
    return query;
}

void ConsoleUI::searchScreen() {
    printTitle("SEARCH DOCUMENTS");

    std::string query = askForQueryWithAutocomplete();
    if (query.empty()) {
        if (!inputEnded_) {
            std::cout << "\n  Empty query. Please type at least one word to search.\n";
            waitForEnter();
        }
        return;
    }

    std::vector<AppServices::SearchResult> results = AppServices::searchDocuments(query);

    history_.addEntry(query, static_cast<int>(results.size()));

    if (results.empty()) {
        std::cout << "\n  No documents matched \"" << query << "\".\n"
                  << "  Tip: try different or fewer words.\n";
        waitForEnter();
        return;
    }
    showResults(query, results);
}

void ConsoleUI::showResults(const std::string& query,
                            const std::vector<AppServices::SearchResult>& results) {
    while (true) {
        std::cout << "\n";
        printLine('=', kLineWidth);
        std::cout << "  RESULTS FOR: \"" << shorten(query, 40) << "\"  (" << results.size()
                  << " found)\n";
        printLine('=', kLineWidth);

        for (std::size_t i = 0; i < results.size(); ++i) {
            const AppServices::SearchResult& r = results[i];

            std::string preview = r.preview;
            if (trim(preview).empty()) {

                AppServices::Document doc;
                if (AppServices::getDocumentById(r.docId, doc)) preview = doc.content;
            }

            std::cout << "  [" << (i + 1) << "] " << shorten(r.title, 45) << "\n"
                      << "      Document ID : " << r.docId << "\n"
                      << "      Relevance   : " << std::fixed << std::setprecision(2) << r.score
                      << "\n"
                      << "      Preview     : " << makePreview(preview) << "\n";
            printLine('-', kLineWidth);
        }

        std::cout << "  Enter a result number to open the full document,\n"
                  << "  or 0 to go back to the main menu.\n\n";
        int choice = readInt("Your choice: ", 0, static_cast<int>(results.size()));
        if (choice <= 0) return;

        openDocument(results[static_cast<std::size_t>(choice - 1)].docId);
    }
}

void ConsoleUI::openDocument(int docId) {
    AppServices::Document doc;
    if (!AppServices::getDocumentById(docId, doc)) {
        std::cout << "\n  [ERROR] Could not open document with ID " << docId << ".\n";
        waitForEnter();
        return;
    }

    std::cout << "\n";
    printLine('=', kLineWidth);
    std::cout << "  DOCUMENT " << doc.id << ": " << doc.title << "\n";
    printLine('=', kLineWidth);
    if (trim(doc.content).empty()) {
        std::cout << "  (This document is empty.)\n";
    } else {
        std::cout << doc.content << "\n";
    }
    printLine('=', kLineWidth);
    waitForEnter();
}

void ConsoleUI::historyScreen() {
    printTitle("SEARCH HISTORY");

    if (history_.isEmpty()) {
        std::cout << "  No searches yet. Use option 3 to search documents.\n";
        waitForEnter();
        return;
    }

    std::cout << "  " << std::left << std::setw(5) << "No." << std::setw(21) << "Date & Time"
              << std::setw(9) << "Results" << "Query\n";
    std::cout << "  ";
    printLine('-', kLineWidth - 2);

    const std::vector<HistoryEntry>& entries = history_.getEntries();
    for (std::size_t i = 0; i < entries.size(); ++i) {
        std::cout << "  " << std::left << std::setw(5) << (i + 1) << std::setw(21)
                  << entries[i].timestamp << std::setw(9) << entries[i].resultCount
                  << shorten(entries[i].query, 30) << "\n";
    }
    std::cout << "\n  Total searches: " << entries.size() << "\n";

    std::cout << "\n  1. Back to main menu\n  2. Clear search history\n\n";
    int choice = readInt("Your choice: ", 1, 2);
    if (choice == 2) {
        history_.clear();
        history_.save();
        std::cout << "\n  Search history cleared.\n";
        waitForEnter();
    }
}

void ConsoleUI::projectInfoScreen() const {
    printTitle("PROJECT INFORMATION");
    std::cout << "  Project Name        : Smart Autocomplete & Search Engine\n"
              << "  Programming Language: C++17\n"
              << "  Supported File Type : TXT\n\n"
              << "  Team Members:\n"
              << "    - Dewang   : Search Engine\n"
              << "    - Ravinder : Autocomplete\n"
              << "    - Piyush   : Document Management\n"
              << "    - Aszad    : Console UI and Search History\n";
    std::cout << "\nPress Enter to continue...";
    std::string ignored;
    std::getline(std::cin, ignored);
}

void ConsoleUI::exitScreen() {
    printTitle("EXIT");
    if (history_.save()) {
        std::cout << "  Search history saved.\n";
    } else {
        std::cout << "  [WARNING] Could not save search history.\n";
    }
    std::cout << "  Thank you for using Smart Autocomplete & Search Engine. Goodbye!\n\n";
    running_ = false;
}
