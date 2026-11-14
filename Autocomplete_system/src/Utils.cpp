#include "Utils.h"

#include <algorithm>
#include <cctype>

namespace Utils {

// Convert text to lowercase
std::string toLowerCase(const std::string& text) {

    std::string result = text;

    std::transform(
        result.begin(),
        result.end(),
        result.begin(),
        [](unsigned char ch) {
            return static_cast<char>(std::tolower(ch));
        }
    );

    return result;
}

// Remove punctuation
std::string removePunctuation(const std::string& text) {

    std::string result;

    for (unsigned char ch : text) {

        if (!std::ispunct(ch)) {
            result += static_cast<char>(ch);
        }
    }

    return result;
}

// Normalize a single word
std::string normalizeWord(const std::string& word) {

    std::string result = toLowerCase(word);

    result = removePunctuation(result);

    return result;
}

// Normalize multiple words
std::vector<std::string> normalizeWords(
    const std::vector<std::string>& words
) {

    std::vector<std::string> result;

    for (const std::string& word : words) {

        std::string normalized = normalizeWord(word);

        if (!normalized.empty()) {
            result.push_back(normalized);
        }
    }

    return result;
}

}