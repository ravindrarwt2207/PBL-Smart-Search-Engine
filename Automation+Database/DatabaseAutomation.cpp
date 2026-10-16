// DatabaseAutomation.cpp
#include "DatabaseAutomation.h"
#include "sqlite3.h"
#include <iostream>
#include <fstream>
using namespace std;

DatabaseAutomation::DatabaseAutomation(const string &dbName) {
    db = nullptr;
    if (sqlite3_open(dbName.c_str(), &db) != SQLITE_OK) {
        cout << "Database connection failed: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);   // safe even if db is not fully opened
        db = nullptr;
        return;
    }
    if (!createTable()) {
        sqlite3_close(db);
        db = nullptr;
    }
}

DatabaseAutomation::~DatabaseAutomation() {
    if (db != nullptr)
        sqlite3_close(db);
}

bool DatabaseAutomation::isConnected() const {
    return db != nullptr;
}

bool DatabaseAutomation::createTable() {
    const char *sql =
        "CREATE TABLE IF NOT EXISTS documents ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "filename TEXT,"
        "content TEXT);";
    char *errorMessage = nullptr;
    if (sqlite3_exec(db, sql, nullptr, nullptr, &errorMessage) != SQLITE_OK) {
        cout << "Table creation failed: " << errorMessage << endl;
        sqlite3_free(errorMessage);
        return false;
    }
    return true;
}

// ---------- Automation ----------

void DatabaseAutomation::addTask(const string &filename) {
    Task task;
    task.filename = filename;
    taskQueue.push(task);
}

int DatabaseAutomation::processTasks() {
    int savedCount = 0;
    while (!taskQueue.empty()) {
        Task task = taskQueue.front();   // oldest task first (FIFO)
        taskQueue.pop();

        string content;
        if (!readFile(task.filename, content)) {
            cout << "Could not open file: " << task.filename << endl;
            continue;                    // skip this file, go to the next task
        }
        if (saveDocument(task.filename, content))
            savedCount++;
    }
    return savedCount;
}

bool DatabaseAutomation::readFile(const string &filename, string &content) {
    ifstream file(filename);
    if (!file)
        return false;
    string line;
    content = "";
    while (getline(file, line))
        content += line + " ";
    return true;
}

// ---------- Database ----------

bool DatabaseAutomation::saveDocument(const string &filename, const string &content) {
    if (db == nullptr)
        return false;

    const char *sql = "INSERT INTO documents (filename, content) VALUES (?, ?);";
    sqlite3_stmt *statement = nullptr;
    if (sqlite3_prepare_v2(db, sql, -1, &statement, nullptr) != SQLITE_OK) {
        cout << "Prepare failed: " << sqlite3_errmsg(db) << endl;
        return false;
    }
    sqlite3_bind_text(statement, 1, filename.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(statement, 2, content.c_str(), -1, SQLITE_TRANSIENT);

    bool ok = (sqlite3_step(statement) == SQLITE_DONE);
    if (!ok)
        cout << "Save failed: " << sqlite3_errmsg(db) << endl;
    sqlite3_finalize(statement);
    return ok;
}

vector<Document> DatabaseAutomation::getAllDocuments() {
    vector<Document> documents;
    if (db == nullptr)
        return documents;

    const char *sql = "SELECT id, filename, content FROM documents ORDER BY id;";
    sqlite3_stmt *statement = nullptr;
    if (sqlite3_prepare_v2(db, sql, -1, &statement, nullptr) != SQLITE_OK) {
        cout << "Read failed: " << sqlite3_errmsg(db) << endl;
        return documents;
    }
    while (sqlite3_step(statement) == SQLITE_ROW) {
        Document doc;
        doc.id = sqlite3_column_int(statement, 0);
        // column_text can return nullptr for NULL values, so check first
        const unsigned char *name = sqlite3_column_text(statement, 1);
        const unsigned char *text = sqlite3_column_text(statement, 2);
        doc.filename = name ? reinterpret_cast<const char *>(name) : "";
        doc.content  = text ? reinterpret_cast<const char *>(text) : "";
        documents.push_back(doc);
    }
    sqlite3_finalize(statement);
    return documents;
}
