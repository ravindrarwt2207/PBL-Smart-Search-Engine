
#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <sqlite3.h>

using namespace std;

class DatabaseAutomation {
private:
    sqlite3 *db;
    queue<string> tasks;

public:
    DatabaseAutomation() {
        db = nullptr;

        if (sqlite3_open("smartsearch.db", &db) != SQLITE_OK) {
            cout << "Database connection failed\n";
        } else {
            const char *sql =
                "CREATE TABLE IF NOT EXISTS documents ("
                "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                "filename TEXT, content TEXT);";

            sqlite3_exec(db, sql, nullptr, nullptr, nullptr);
            cout << "Database connected\n";
        }
    }

    ~DatabaseAutomation() {
        if (db != nullptr) {
            sqlite3_close(db);
        }
    }

    void addTask(string filename) {
        tasks.push(filename);
    }

    void processTasks() {
        while (!tasks.empty()) {
            string filename = tasks.front();
            tasks.pop();

            processFile(filename);
        }
    }

    void processFile(string filename) {
        if (db == nullptr) {
            cout << "Database unavailable\n";
            return;
        }

        ifstream file(filename);

        if (!file.is_open()) {
            cout << "File not found: " << filename << endl;
            return;
        }

        string content, line;

        while (getline(file, line)) {
            content += line + "\n";
        }

        file.close();

        sqlite3_stmt *stmt = nullptr;

        const char *sql =
            "INSERT INTO documents (filename, content) VALUES (?, ?);";

        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, filename.c_str(),
                              -1, SQLITE_TRANSIENT);

            sqlite3_bind_text(stmt, 2, content.c_str(),
                              -1, SQLITE_TRANSIENT);

            if (sqlite3_step(stmt) == SQLITE_DONE) {
                cout << "File saved: " << filename << endl;
            } else {
                cout << "Saving failed\n";
            }

            sqlite3_finalize(stmt);
        } else {
            cout << "SQL query preparation failed\n";
        }
    }
};

int main() {
    DatabaseAutomation db;

    db.addTask("test.txt");
    db.processTasks();

    return 0;
}
