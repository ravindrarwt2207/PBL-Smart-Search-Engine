# Smart Autocomplete & Search Engine - Aszad's Module (Console UI + Search History)

## Folder structure
```
SmartSearch/
  main.cpp                    entry point
  ConsoleUI.h / .cpp          menus, input validation, screens
  SearchHistory.h / .cpp      persistent history (search_history.txt)
  AppServices.h               shared interface (declarations only)
  AppServices.cpp.template    ADAPTER TEMPLATE - team must fill in -> AppServices.cpp
  CMakeLists.txt
  tests/TestStubServices.cpp  TEST-ONLY stand-in so the UI can run alone
  DocumentManager.*  SearchEngine.*  Autocomplete.*   <- teammates' real files
```

## What is missing for a full build
The project will NOT link until these exist (they are not Aszad's files):
1. Piyush: DocumentManager.h/.cpp  (import, queue, list, get by ID)
2. Dewang: SearchEngine.h/.cpp     (ranked search)
3. Ravinder: Autocomplete.h/.cpp   (Trie suggestions)
4. AppServices.cpp: copy AppServices.cpp.template and replace each TODO with a
   call to the real class. If a teammate's function names/types differ from
   AppServices.h, convert them inside this adapter - do not change the UI.

## Compile
UI-only test build (stand-in modules, NOT the real integration):
    g++ -std=c++17 -Wall -Wextra -I. main.cpp ConsoleUI.cpp SearchHistory.cpp tests/TestStubServices.cpp -o ui_test

Full build with g++:
    g++ -std=c++17 -Wall -Wextra main.cpp ConsoleUI.cpp SearchHistory.cpp AppServices.cpp DocumentManager.cpp SearchEngine.cpp Autocomplete.cpp -o SmartSearch

Full build with CMake (uncomment the teammates' files in CMakeLists.txt first):
    cmake -S . -B build && cmake --build build && ./build/SmartSearch

## Test checklist (run the program from the folder that holds your .txt files)
1. Option 1, valid .txt path            -> SUCCESS + "Indexing complete"
2. Option 1, "notes.pdf" / missing file -> FAILED message, no crash
3. Option 2                             -> ID + title list (or "No documents")
4. Option 3, one word                   -> suggestions + ranked results
5. Option 3, several words              -> suggestions for LAST word only
6. Option 3, unknown word               -> "No documents matched"
7. Option 3, pick a suggestion number   -> last word replaced
8. Open a result by number; 0 goes back
9. Exit (6) -> search_history.txt written
10. Restart, option 4 -> old searches still listed
11. Type "abc", "0", "99" at any menu   -> re-prompt, no crash
12. Press Enter on empty inputs         -> friendly message
13. Ctrl+D (Ctrl+Z on Windows) at any prompt -> saves history and exits

## Limitation (autocomplete)
Standard C++ input is line-based; reading single key presses is not portable.
So suggestions appear AFTER the user presses Enter on the query, for the last
word, and the user may pick one by number. This is reliable on every OS.

## Viva: what each function does
SearchHistory
- SearchHistory(file)     remembers the history file name
- load()                  reads file; skips damaged lines; false if no file yet
- save()                  writes all entries, one per line; false on write error
- addEntry(q, n)          adds query + result count + current time
- currentTimestamp()      formats local time "YYYY-MM-DD HH:MM:SS"
- cleanText()             removes line breaks so one search = one line
- getEntries/size/isEmpty/clear   simple accessors
ConsoleUI
- run()                   loads history, loops: show menu -> read choice -> call screen
- showHeader/showMainMenu draw the banner and numbered menu
- addDocumentScreen()     validates path + .txt, imports, then processes indexing
- viewAllDocumentsScreen() lists ID + title, handles empty list
- searchScreen()          query -> autocomplete -> search -> save to history -> results
- askForQueryWithAutocomplete() gets query, shows <=5 suggestions for last word
- showResults()           ranked list (title, ID, score, preview); pick number to open
- openDocument()          fetches by ID and prints full text; handles missing record
- historyScreen()         table of past searches (oldest first); optional clear
- projectInfoScreen()     project, language, format, team
- exitScreen()            saves history, stops the loop
- readInt(prompt,min,max) repeats until a valid number in range (digits only)
- readLine()              getline wrapper; detects closed input
- trim/stripQuotes/toLower/hasTxtExtension/makePreview/shorten/lastWord/replaceLastWord
                          small text helpers
- printLine/printTitle    ASCII borders
AppServices (adapter): one function per module call (see AppServices.h comments).

## Sample output (from the UI-only test build with stand-in modules)
```
============================================================
  RESULTS FOR: "data structure"  (1 found)
============================================================
  [1] ds.txt
      Document ID : 101
      Relevance   : 6.00
      Preview     : Data structures organize and store data. Data structures include...
------------------------------------------------------------
  Enter a result number to open the full document,
  or 0 to go back to the main menu.
```
