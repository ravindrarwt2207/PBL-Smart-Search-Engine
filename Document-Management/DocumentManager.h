#pragma once

#include <string>
#include <vector>
#include <queue>
#include "model.h"

// DocumentManager: import, store, list, retrieve documents and manage
// a FIFO queue of document IDs waiting for indexing.
//
// This class does NOT do tokenization, indexing, searching, ranking,
// autocomplete, or UI. Those belong to other teammates.
class DocumentManager {
public:
    DocumentManager();

    // Call once at startup. Creates managed folder, loads catalogue,
    // rebuilds the pending-indexing queue.
    // Returns false only if the managed folder cannot be created/opened.
    bool initialize(std::string &message);

    // Imports a .txt file from sourcePath.
    // On success: newId gets the assigned positive ID.
    // On failure: returns false and message explains why.
    bool importDocument(const std::string &sourcePath,
                        int &newId,
                        std::string &message);

    // Prints all stored documents (ID, pending status, size, title).
    void listDocuments() const;

    // Retrieves a document by ID. Returns false if ID is invalid.
    bool getDocument(int id, Document &outDocument) const;

    // Pending indexing queue helpers.
    // getNextPendingId does NOT remove the ID. Teammate must call
    // markIndexed() only after successful indexing.
    bool getNextPendingId(int &id) const;

    // Convenience: get next pending document directly.
    bool getNextPendingDocument(Document &outDocument) const;

    // Marks a document as indexed and removes it from pending work.
    // Safe to call after successful indexing only.
    bool markIndexed(int id, std::string &message);

    std::size_t pendingCount() const;
    std::size_t documentCount() const;

private:
    std::vector<Document> documents;       // all stored documents
    std::vector<bool> pendingStatus;       // parallel to documents
    std::queue<int> pendingQueue;          // FIFO IDs waiting for indexing
    int nextId;                            // next available positive ID
    std::string storageFile;               // catalogue file path
    std::string managedFolder;             // managed documents folder

    bool saveCatalogue(std::string &message);
    void rebuildQueue();
    bool isDuplicateContent(const std::string &content) const;
    std::string managedPathForId(int id) const;
};