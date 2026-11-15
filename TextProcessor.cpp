#include "TextProcessor.h"

std::vector<std::string> TextProcessor::tokenize(
    const std::string& text
) {
    std::vector<std::string> words;
    std::string word;

    for (char ch : text) {
        // Convert uppercase English letters to lowercase.
        if (ch >= 'A' && ch <= 'Z') {
            ch = ch - 'A' + 'a';
        }

        // Keep English letters and digits.
        if ((ch >= 'a' && ch <= 'z') ||
            (ch >= '0' && ch <= '9')) {
            word += ch;
        } else {
            // Spaces and punctuation separate words.
            if (!word.empty()) {
                words.push_back(word);
                word.clear();
            }
        }
    }

    // Save the final word if text ends without a separator.
    if (!word.empty()) {
        words.push_back(word);
    }

    return words;
}