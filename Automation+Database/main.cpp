#include <iostream>
#include <fstream>
#include <queue>
#include <string>
#include "sqlite3.h"
using namespace std;
struct Task { // Task structure
    string filename;
};
class AutomationDatabase {// Automation + Database class
private:
    queue<Task> taskQueue;
    sqlite3 *db;
public:
    AutomationDatabase() {
        db = nullptr;

        if (sqlite3_open("smartsearch.db", &db) == SQLITE_OK) {
            cout << "Database connected successfully.\n";
            createTable();
        }
        else
            cout << "Database connection failed.\n";
    }
    ~AutomationDatabase() {
        if (db != nullptr) 
            sqlite3_close(db);
    }
    void createTable() {
        string sql =
            "CREATE TABLE IF NOT EXISTS documents ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "filename TEXT,"
            "content TEXT);";
        char *errorMessage = nullptr;
    if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errorMessage)
            == SQLITE_OK)
            cout << "Documents table ready.\n";
        else {
            cout << "Table creation failed.\n";
            sqlite3_free(errorMessage);
        }
    }
    void addTask(string filename) {// Add new task to queue
        Task task;
        task.filename = filename;
        taskQueue.push(task);
        cout << "\nTask added to automation queue.\n";
        cout << "File: " << filename << endl;
    }
    void processTasks() {// Process all tasks
        while (!taskQueue.empty()) {
            Task task = taskQueue.front();
            taskQueue.pop();
            cout << "\nProcessing: "
                 << task.filename << endl;
            processFile(task.filename);
        }
    }
    void processFile(string filename) { // Read file
        ifstream file(filename);
        if (!file) {
            cout << "File could not be opened.\n";
            return;
        }
        string content;
        string line;
        while (getline(file, line)) 
            content += line + " ";
        file.close();
        cout << "File processed successfully.\n";// Save processed document
        saveToDatabase(filename, content);
    }
    void saveToDatabase(string filename, string content) {    // Save document into SQLite
        string sql =
            "INSERT INTO documents (filename, content) "
            "VALUES (?, ?);";
        sqlite3_stmt *statement;
        if (sqlite3_prepare_v2(
                db,
                sql.c_str(),
                -1,
                &statement,
                nullptr) != SQLITE_OK) {
            cout << "Database statement failed.\n";
            return;
        }
        sqlite3_bind_text(
            statement,
            1,
            filename.c_str(),
            -1,
            SQLITE_TRANSIENT
        );
        sqlite3_bind_text(
            statement,
            2,
            content.c_str(),
            -1,
            SQLITE_TRANSIENT
        );

        if (sqlite3_step(statement) == SQLITE_DONE) 
            cout << "Document saved to database.\n";
        else 
            cout << "Failed to save document.\n";
        sqlite3_finalize(statement);
    }
   void viewDatabase() {
    string sql =
        "SELECT id, filename, content FROM documents;";

    sqlite3_stmt *statement;

    if (sqlite3_prepare_v2(
            db,
            sql.c_str(),
            -1,
            &statement,
            nullptr) != SQLITE_OK) {

        cout << "Unable to read database.\n";
        return;
    }

    cout << "\n========== DATABASE DOCUMENTS ==========\n";

    while (sqlite3_step(statement) == SQLITE_ROW) {

        int id = sqlite3_column_int(statement, 0);

        const unsigned char *filename =
            sqlite3_column_text(statement, 1);

        const unsigned char *content =
            sqlite3_column_text(statement, 2);

        cout << "\nID       : " << id;
        cout << "\nFilename : " << filename;
        cout << "\nContent  : " << content;
        cout << "\n----------------------------------------\n";
    }

    sqlite3_finalize(statement);
}
};
int main() {// Testing the Automation + Database module
    AutomationDatabase automation;
    string filename;
    cout << "\nEnter file name to process: ";
    getline(cin, filename);
    automation.addTask(filename);  // Upload → Task Queue
    automation.processTasks(); // Task Queue → Processing → Database
    automation.viewDatabase();    // Show saved documents
    return 0;
}
