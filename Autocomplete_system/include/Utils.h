#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

namespace Utils {

    std::string toLowerCase(
        const std::string& text
    );

    std::string removePunctuation(
        const std::string& text
    );

    std::string normalizeWord(
        const std::string& word
    );

    std::vector<std::string> normalizeWords(
        const std::vector<std::string>& words
    );

}

#endif