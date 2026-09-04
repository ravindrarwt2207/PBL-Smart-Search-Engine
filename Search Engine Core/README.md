#include <iostream>
#include <vector>
#include <string>
using namespace std;

class SearchEngine {
private:
    vector<string> documents;

public:

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

        for (int i = 0; i < documents.size(); i++) {

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

        if (documents.empty()) {
            cout << "No documents available.\n";
        }
        else {
            for (int i = 0; i < documents.size(); i++) {
                cout << i + 1 << ". "
                     << documents[i] << endl;
            }
        }
    }

    void upload() {
        string name;

        cout << "\n========== UPLOAD ==========\n";
        cout << "Enter document name: ";
        cin.ignore();
        getline(cin, name);

        documents.push_back(name);

        cout << "Document uploaded successfully!\n";
    }

    void adminDashboard() {
        cout << "\n========== ADMIN DASHBOARD ==========\n";

        cout << "Total Documents : "
             << documents.size() << endl;

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

    // Sample documents
    engine.upload();
    engine.upload();

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
