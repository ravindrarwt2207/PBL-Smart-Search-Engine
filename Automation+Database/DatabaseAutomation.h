// DatabaseAutomation.h
// Module: Database + Automation (SQLite storage and FIFO file queue)
#ifndef DATABASE_AUTOMATION_H
#define DATABASE_AUTOMATION_H

#include <string>
#include <queue>
#include <vector>

struct sqlite3; // forward declaration, so teammates don't need sqlite3.h

// One stored document (this is what the search engine receives)
struct Document {
    int id;
    std::string filename;
    std::string content;
};

struct Task { // one automation task = one file to process
    std::string filename;
};

class DatabaseAutomation {
private:
    std::queue<Task> taskQueue; // FIFO queue
    sqlite3 *db;

    bool createTable();
    bool readFile(const std::string &filename, std::string &content);

public:
    DatabaseAutomation(const std::string &dbName = "smartsearch.db");
    ~DatabaseAutomation();

    // The object owns the database connection, so it must not be copied.
    DatabaseAutomation(const DatabaseAutomation &) = delete;
    DatabaseAutomation &operator=(const DatabaseAutomation &) = delete;

    bool isConnected() const;

    // Automation (FIFO queue)
    void addTask(const std::string &filename);  // add a file to the queue
    int processTasks();                         // process all queued files, returns how many were saved

    // Database
    bool saveDocument(const std::string &filename, const std::string &content);
    std::vector<Document> getAllDocuments();    // for the search engine
};

#endif
