
#ifndef DATABASE_AUTOMATION_H
#define DATABASE_AUTOMATION_H

#include <string>
#include <queue>
#include <vector>

struct sqlite3;

struct Document {
    int id;
    std::string filename;
    std::string content;
};

struct Task { 
    std::string filename;
};

class DatabaseAutomation {
private:
    std::queue<Task> taskQueue;
    sqlite3 *db;

    bool createTable();
    bool readFile(const std::string &filename, std::string &content);

public:
    DatabaseAutomation(const std::string &dbName = "smartsearch.db");
    ~DatabaseAutomation();

   
    DatabaseAutomation(const DatabaseAutomation &) = delete;
    DatabaseAutomation &operator=(const DatabaseAutomation &) = delete;

    bool isConnected() const;

  
    void addTask(const std::string &filename); 
    int processTasks();                         

    bool saveDocument(const std::string &filename, const std::string &content);
    std::vector<Document> getAllDocuments(); 
};

#endif
