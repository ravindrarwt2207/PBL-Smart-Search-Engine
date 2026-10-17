#include <iostream>
#include <string>
#include <limits>
#include <sstream>
#include <cctype>
using namespace std;
struct Document
{
    int id;
    string title;
    string content;
};
struct SearchResult
{
    int documentIndex;
    int frequency;
};
string toLowerCase(string text)
{
    for (int i = 0; i < (int)text.length(); i++)
    {
        text[i] = static_cast<char>(
            tolower(static_cast<unsigned char>(text[i]))
        );
    }

    return text;
}
bool containsWord(string text, const string& keyword)
{
    text = toLowerCase(text);

    for (int i = 0; i < (int)text.length(); i++)
    {
        if (!isalnum(static_cast<unsigned char>(text[i])))
        {
            text[i] = ' ';
        }
    }

    stringstream input(text);
    string word;

    while (input >> word)
    {
        if (word == keyword)
        {
            return true;
        }
    }

    return false;
}
int countWord(string text, const string& keyword)
{
    text = toLowerCase(text);

    for (int i = 0; i < (int)text.length(); i++)
    {
        if (!isalnum(static_cast<unsigned char>(text[i])))
        {
            text[i] = ' ';
        }
    }

    stringstream input(text);
    string word;
    int frequency = 0;

    while (input >> word)
    {
        if (word == keyword)
        {
            frequency++;
        }
    }

    return frequency;
}
void displayDocuments(const Document documents[], int count)
{
    for (int i = 0; i < count; i++)
    {
        cout << "\nID: " << documents[i].id << "\n";
        cout << "Title: " << documents[i].title << "\n";
        cout << "Content: " << documents[i].content << "\n";
    }
}
void sortResults(SearchResult results[], int count)
{
    for (int i = 1; i < count; i++)
    {
        SearchResult current = results[i];
        int j = i - 1;

        while (j >= 0 && results[j].frequency < current.frequency)
        {
            results[j + 1] = results[j];
            j--;
        }

        results[j + 1] = current;
    }
}
void searchDocuments(const Document documents[], int count,
                     const string& keyword)
{
    string lowerKeyword = toLowerCase(keyword);

    if (lowerKeyword.empty())
    {
        cout << "Please enter a keyword.\n";
        return;
    }

    for (int i = 0; i < (int)lowerKeyword.length(); i++)
    {
        if (!isalnum(static_cast<unsigned char>(lowerKeyword[i])))
        {
            cout << "Enter one word using letters or numbers only.\n";
            return;
        }
    }

    const int MAX_RESULTS = 100;

    if (count > MAX_RESULTS)
    {
        cout << "Too many documents for the result array.\n";
        return;
    }

    SearchResult results[MAX_RESULTS];
    int resultCount = 0;

    for (int i = 0; i < count; i++)
    {
        int frequency =
            countWord(documents[i].title, lowerKeyword) +
            countWord(documents[i].content, lowerKeyword);

        if (frequency > 0)
        {
            results[resultCount].documentIndex = i;
            results[resultCount].frequency = frequency;
            resultCount++;
        }
    }

    if (resultCount == 0)
    {
        cout << "No matching documents found.\n";
        return;
    }

    sortResults(results, resultCount);

    cout << "\nResults found: " << resultCount << "\n";

    for (int i = 0; i < resultCount; i++)
    {
        int index = results[i].documentIndex;

        cout << "\nRank: " << i + 1 << "\n";
        cout << "ID: " << documents[index].id << "\n";
        cout << "Title: " << documents[index].title << "\n";
        cout << "Content: " << documents[index].content << "\n";
        cout << "Frequency: " << results[i].frequency << "\n";
    }
}
int main()
{
    const int DOCUMENT_COUNT = 8;

Document documents[DOCUMENT_COUNT] = {
    {1, "Arrays",
     "An array stores elements in consecutive memory locations."},

    {2, "Linear Search",
     "Linear search checks each element one by one."},

    {3, "Binary Search",
     "Binary search works on a sorted array. This search halves the search range."},

    {4, "Linked List",
     "A linked list contains nodes. Each node stores data and a link to the next node."},

    {5, "Stack",
     "A stack follows last in first out. A stack supports push and pop operations."},

    {6, "Queue",
     "A queue follows first in first out. A queue supports enqueue and dequeue operations."},

    {7, "Insertion Sort",
     "Insertion sort places each element in its correct position. This sort works well for small datasets."},

    {8, "Graph",
     "A graph contains vertices and edges. Graph traversal can use breadth first search."}
};

    int choice = 0;

    while (choice != 3)
    {
        cout << "\n--- Search Engine Core ---\n";
        cout << "1. Search\n";
        cout << "2. View documents\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Please enter a number.\n";
            continue;
        }

        switch (choice)
        {
            case 1:{
    string keyword;

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter a keyword: ";
    getline(cin, keyword);

    if (keyword.empty())
    {
        cout << "Please enter a keyword.\n";
    }
    else
    {
        searchDocuments(documents, DOCUMENT_COUNT, keyword);
    }

    break;
}
            case 2:
                displayDocuments(documents, DOCUMENT_COUNT);
                break;
            case 3:
                cout << "Program closed.\n";
                break;

            default:
                cout << "Invalid choice. Enter 1, 2 or 3.\n";
        }
    }

    return 0;
}