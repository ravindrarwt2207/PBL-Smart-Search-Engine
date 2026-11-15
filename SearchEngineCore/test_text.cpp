#include <iostream>
#include "TextProcessor.h"

int main() {
    std::string text;

    std::cout << "Enter text: ";
    std::getline(std::cin, text);

    std::vector<std::string> words =
        TextProcessor::tokenize(text);

    std::cout << "Extracted words:\n";

    for (const std::string& word : words) {
        std::cout << word << '\n';
    }

    return 0;
}
