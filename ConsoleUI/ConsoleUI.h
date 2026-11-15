// ============================================================================
// ConsoleUI.h  -  Menu-driven console interface (Aszad)
// ============================================================================
#ifndef CONSOLE_UI_H
#define CONSOLE_UI_H

#include <string>

#include "AppServices.h"
#include "SearchHistory.h"

class ConsoleUI {
public:
    ConsoleUI();

    // Starts the program: loads history, shows the main menu until Exit.
    void run();

private:
    // ---- Screens -----------------------------------------------------------
    void showHeader() const;
    void showMainMenu() const;
    void addDocumentScreen();
    void viewAllDocumentsScreen();
    void searchScreen();
    void historyScreen();
    void projectInfoScreen() const;
    void exitScreen();

    // ---- Search helpers ----------------------------------------------------
    std::string askForQueryWithAutocomplete();
    void        showResults(const std::string& query,
                            const std::vector<AppServices::SearchResult>& results);
    void        openDocument(int docId);

    // ---- Input helpers (all input is validated here) -----------------------
    bool readLine(std::string& line);                         // false on end-of-input
    int  readInt(const std::string& prompt, int min, int max); // -1 on end-of-input
    void waitForEnter();

    // ---- Text helpers --------------------------------------------------------
    static std::string trim(const std::string& text);
    static std::string stripQuotes(const std::string& text);
    static std::string toLower(const std::string& text);
    static bool        hasTxtExtension(const std::string& path);
    static std::string makePreview(const std::string& text, std::size_t maxLength = 100);
    static std::string shorten(const std::string& text, std::size_t maxLength);
    static std::string lastWord(const std::string& query);
    static std::string replaceLastWord(const std::string& query, const std::string& word);

    // ---- Drawing helpers -----------------------------------------------------
    static void printLine(char symbol = '=', int width = 60);
    static void printTitle(const std::string& title);

    SearchHistory history_;
    bool          inputEnded_;   // true if the user closed the input (Ctrl+D / Ctrl+Z)
    bool          running_;
};

#endif  // CONSOLE_UI_H
