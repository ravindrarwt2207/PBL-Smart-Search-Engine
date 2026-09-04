#include <iostream>
#include <string>
using namespace std;

class SearchEngine {
private:
    string documents[100];   // Array can store maximum 100 documents
    int documentCount;

public:

    // Constructor
    SearchEngine() {
        documentCount = 0;
    }

    void home() {
        cout << "\n========== HOME ==========\n";
        cout << "Welcome to My Search Engine\n";
        cout << "1. Search\n";
        cout << "2. View Documents\n";
        cout << "3. Upload Document\n";
        cout << "4. Admin Dashboard\n";
        cout << "5. Automation Status\n";
        cout << "6. Exit\n";
    }

    void search() {
        string keyword;

        cout << "\n========== SEARCH RESULTS ==========\n";
        cout << "Enter keyword: ";
        cin >> keyword;

        bool found = false;

        for (int i = 0; i < documentCount; i++) {

            if (documents[i].find(keyword) != string::npos) {
                cout << "Result: " << documents[i] << endl;
                found = true;
            }
        }

        if (!found) {
            cout << "No results found.\n";
        }
    }

    void document() {
        cout << "\n========== DOCUMENT ==========\n";

        if (documentCount == 0) {
            cout << "No documents available.\n";
        }
        else {
            for (int i = 0; i < documentCount; i++) {
                cout << i + 1 << ". "
                     << documents[i] << endl;
            }
        }
    }

    void upload() {
        string name;

        cout << "\n========== UPLOAD ==========\n";

        if (documentCount >= 100) {
            cout << "Document storage is full.\n";
            return;
        }

        cout << "Enter document name: ";
        cin.ignore();
        getline(cin, name);

        documents[documentCount] = name;
        documentCount++;

        cout << "Document uploaded successfully!\n";
    }

    void adminDashboard() {
        cout << "\n========== ADMIN DASHBOARD ==========\n";

        cout << "Total Documents : "
             << documentCount << endl;

        cout << "Search Engine    : Active\n";
        cout << "Database         : Connected\n";
        cout << "Users            : 1\n";
    }

    void automationStatus() {
        cout << "\n========== AUTOMATION STATUS ==========\n";

        cout << "Document Indexing : Running\n";
        cout << "Search Database   : Online\n";
        cout << "Automatic Backup  : Active\n";
        cout << "Web Crawler       : Stopped\n";
    }
};


int main() {

    SearchEngine engine;

    int choice;

    while (true) {

        engine.home();

        cout << "\nEnter your choice: ";
        cin >> choice;

        switch (choice) {

        case 1:
            engine.search();
            break;

        case 2:
            engine.document();
            break;

        case 3:
            engine.upload();
            break;

        case 4:
            engine.adminDashboard();
            break;

        case 5:
            engine.automationStatus();
            break;

        case 6:
            cout << "\nThank you for using the Search Engine!\n";
            return 0;

        default:
            cout << "\nInvalid choice!\n";
        }
    }

    return 0;
}
