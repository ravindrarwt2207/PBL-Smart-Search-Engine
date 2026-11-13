#pragma once

#include <string>
#include <vector>
#include <queue>
#include "model.h"
class DocumentManager {
public:
    DocumentManager();

    bool initialize(std::string &message);


    bool importDocument(const std::string &sourcePath,
                        int &newId,
                        std::string &message);


    void listDocuments() const;


    bool getDocument(int id, Document &outDocument) const;


    bool getNextPendingId(int &id) const;


    bool getNextPendingDocument(Document &outDocument) const;

    bool markIndexed(int id, std::string &message);

    std::size_t pendingCount() const;
    std::size_t documentCount() const;

private:
    std::vector<Document> documents;      
    std::vector<bool> pendingStatus;      
    std::queue<int> pendingQueue;         
    int nextId;                            
    std::string storageFile;              
    std::string managedFolder;             

    bool saveCatalogue(std::string &message);
    void rebuildQueue();
    bool isDuplicateContent(const std::string &content) const;
    std::string managedPathForId(int id) const;
};
