#pragma once

#include <string>
#include <vector>
#include <queue>
#include<iostream>
#include "model.h"
using namespace std;
class DocumentManager {
public:
    DocumentManager();
    bool initialize(string &message);
    bool importDocument(const string &sourcePath,int &newId,string &message);
    void listDocuments() const;
    bool getDocument(int id, Document &outDocument) const;
    bool getNextPendingId(int &id) const;
    bool getNextPendingDocument(Document &outDocument) const;
    bool markIndexed(int id, string &message);
  size_t pendingCount() const;
    size_t documentCount() const;
private:
    vector<Document> documents;      
   vector<bool> pendingStatus;      
    queue<int> pendingQueue;          
    int nextId;                          
    string storageFile;               
    string managedFolder;             

    bool saveCatalogue(string &message);
    void rebuildQueue();
    bool isDuplicateContent(const string &content) const;
    string managedPathForId(int id) const;
};
