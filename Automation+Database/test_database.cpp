#include <iostream>
#include "DatabaseAutomation.h"
using namespace std;

int main(int argc, char *argv[]) {
    DatabaseAutomation dbAuto;        
    if (!dbAuto.isConnected())
        return 1;

    for (int i = 1; i < argc; i++)
        dbAuto.addTask(argv[i]);        

    cout << "Saved " << dbAuto.processTasks() << " file(s).\n";

    vector<Document> docs = dbAuto.getAllDocuments();
    cout << "Documents in database: " << docs.size() << "\n";
    for (const Document &d : docs)
        cout << d.id << " | " << d.filename << " | " << d.content << "\n";
    return 0;
}
