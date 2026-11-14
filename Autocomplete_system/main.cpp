
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <termios.h>
#include <unistd.h>

#include "SuggestionEngine.h"
#include "Utils.h"

using namespace std;

int main() {

    SuggestionEngine engine;
    vector<string> words;

    // Load vocabulary
    ifstream file("data/Sample_words.txt");

    if (!file.is_open()) {
        cout << "Error: Cannot open vocabulary file!" << endl;
        return 1;
    }

    string word;

    while (file >> word) {
        words.push_back(word);
    }

    file.close();

    // Normalize vocabulary
    vector<string> normalizedWords =
        Utils::normalizeWords(words);

    // Insert words into Trie
    engine.addWords(normalizedWords);

    cout << "================================\n";
    cout << "      AUTOCOMPLETE SYSTEM       \n";
    cout << "================================\n";

    cout << "Total words: "
         << normalizedWords.size() << endl;

    cout << "\nStart typing your word:\n";
    cout << "(Press Enter to finish, Backspace to delete)\n";

    // Enable real-time keyboard input
    termios oldt, newt;

    tcgetattr(STDIN_FILENO, &oldt);

    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);

    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    string prefix = "";

    while (true) {

        cout << "\033[2J\033[H";

        cout << "================================\n";
        cout << "      AUTOCOMPLETE SYSTEM       \n";
        cout << "================================\n";

        cout << "\nSearch: " << prefix << "|";

        cout << "\n\nSuggestions:\n";
        cout << "-------------------------\n";

        if (!prefix.empty()) {

            vector<string> suggestions =
                engine.suggest(prefix, 5);

            if (suggestions.empty()) {

                cout << "No suggestions found.\n";

            } else {

                for (int i = 0; i < suggestions.size(); i++) {

                    cout << i + 1 << ". "
                         << suggestions[i]
                         << " (Frequency: "
                         << engine.getWordFrequency(
                                suggestions[i])
                         << ")\n";
                }
            }

        } else {
            cout << "Start typing to see suggestions.\n";
        }

        cout << "\nPress Enter to exit.\n";

        char ch = getchar();

        if (ch == '\n' || ch == '\r') {
            break;
        }

        // Handle Backspace
        if (ch == 127 || ch == 8) {

            if (!prefix.empty()) {
                prefix.pop_back();
            }

        }

        // Handle normal characters
        else if ((ch >= 'a' && ch <= 'z') ||
                 (ch >= 'A' && ch <= 'Z')) {

            prefix += ch;
        }
    }

    // Restore normal terminal settings
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    cout << "\n\nFinal search: " << prefix << endl;

    cout << "Autocomplete session completed!\n";

    return 0;
}
