#include "DocumentManager.h"

#include <fstream>
#include <iostream>
#include <filesystem>
#include <cctype>
#include <sstream>
#include <limits>
using namespace std;
namespace fs = filesystem;
static bool parseInt(const string &text, int &value) {
    istringstream iss(text);
    if (!(iss >> value)) {
        return false;
    }
    char extra;
    if (iss >> extra) {
        return false;
    }
    return true;
}
static bool parseLongLong(const string &text, long long &value) {
    istringstream iss(text);
    if (!(iss >> value)) {
        return false;
    }
    char extra;
    if (iss >> extra) {
        return false;
    }
    return true;
}
DocumentManager::DocumentManager()
    : nextId(1),
      storageFile("document_catalogue.dat"),
      managedFolder("managed_documents") {
}
bool DocumentManager::initialize(string &message) {
    documents.clear();
    pendingStatus.clear();
    while (!pendingQueue.empty()) {
        pendingQueue.pop();
    }
    nextId = 1;
   error_code ec;
    fs::create_directories(managedFolder, ec);
    if (ec) {
        message = "Could not create managed documents folder: " + ec.message();
        return false;
    }
    string tempFile = storageFile + ".tmp";
    error_code recoverEc;
    bool storageExists = fs::exists(storageFile, recoverEc);
    bool tempExists = fs::exists(tempFile, recoverEc);

    if (!storageExists && tempExists) {
        error_code renameEc;
        fs::rename(tempFile, storageFile, renameEc);
    }
    error_code existsEc;
    if (!fs::exists(storageFile, existsEc)) {
        message = "No saved catalogue found. Starting with an empty document list.";
        return true;
    }
    ifstream in(storageFile, ios::binary);
    if (!in) {
        message = "Catalogue file exists but could not be opened for reading.";
        return false;
    }

    bool corrupt = false;
    string line;
    int expectedCount = 0;
    if (!getline(in, line) ||
        line.rfind("NEXTID ", 0) != 0 ||
        !parseInt(line.substr(7), nextId) ||
        nextId < 1) {
        corrupt = true;
    }
    if (!corrupt) {
        if (!getline(in, line) ||
            line.rfind("COUNT ", 0) != 0 ||
            !parseInt(line.substr(6), expectedCount) ||
            expectedCount < 0) {
            corrupt = true;
        }
    }
    if (!corrupt) {
        for (int i = 0; i < expectedCount; ++i) {
            int id = 0;
            int pending = 0;
            string title;
            long long contentSize = 0;

            if (!getline(in, line) || line != "DOC") {
                corrupt = true;
                break;
            }
            if (!getline(in, line) ||
                line.rfind("ID ", 0) != 0 ||
                !parseInt(line.substr(3), id) ||
                id < 1) {
                corrupt = true;
                break;
            }

            if (!getline(in, line) ||
                line.rfind("PENDING ", 0) != 0 ||
                !parseInt(line.substr(8), pending) ||
                (pending != 0 && pending != 1)) {
                corrupt = true;
                break;
            }

            if (!getline(in, line) || line.rfind("TITLE ", 0) != 0) {
                corrupt = true;
                break;
            }
            title = line.substr(6);

            if (!getline(in, line) ||
                line.rfind("SIZE ", 0) != 0 ||
                !parseLongLong(line.substr(5), contentSize) ||
                contentSize < 0) {
                corrupt = true;
                break;
            }
            string content;
            if (contentSize > 0) {
                content.resize(static_cast<size_t>(contentSize));
                in.read(&content[0],
                        static_cast<streamsize>(contentSize));
                if (in.gcount() != static_cast<streamsize>(contentSize)) {
                    corrupt = true;
                    break;
                }
            }
            string dummy;
            getline(in, dummy);

            if (!getline(in, line) || line != "END") {
                corrupt = true;
                break;
            }
            Document doc;
            doc.id = id;
            doc.title = title;
            doc.content = content;
            documents.push_back(doc);
            pendingStatus.push_back(pending == 1);
        }
    }
    in.close();

    if (corrupt || static_cast<int>(documents.size()) != expectedCount) {
        error_code backupEc;
        string backupName = storageFile + ".corrupt";
        error_code removeEc;
        fs::remove(backupName, removeEc); 
        fs::rename(storageFile, backupName, backupEc);
        documents.clear();
        pendingStatus.clear();
        nextId = 1;
        message = "Catalogue was corrupt or incomplete. It has been backed up to "
                  + backupName + ". Starting fresh.";
        return true;
    }
    rebuildQueue();
    message = "Catalogue loaded successfully.";
    return true;
}
bool DocumentManager::importDocument(const string &sourcePath,int &newId,string &message) {
    newId = 0;
    fs::path source(sourcePath);
    error_code ec;
    if (!fs::exists(source, ec) || ec) {
        message = "Source file does not exist.";
        return false;
    }
    if (!fs::is_regular_file(source, ec) || ec) {
        message = "Source path is not a regular file.";
        return false;
    }

    string ext = source.extension().string();
    for (char &c : ext) {
        c = static_cast<char>(tolower(static_cast<unsigned char>(c)));
    }
    if (ext != ".txt") {
        message = "Only .txt files are supported.";
        return false;
    }

    ifstream in(source, ios::binary);
    if (!in) {
        message = "Could not open source file for reading.";
        return false;
    }

    in.seekg(0, ios::end);
    streampos endPos = in.tellg();
    if (endPos < 0) {
        message = "Could not determine source file size.";
        return false;
    }
    streamsize size = static_cast<streamsize>(endPos);
    in.seekg(0, ios::beg);
    string content;
    if (size > 0) {
        content.resize(static_cast<size_t>(size));
        in.read(&content[0], size);
        if (!in) {
            message = "Failed to read complete source file.";
            return false;
        }
    }
    in.close();
    if (content.empty()) {
        message = "Document is empty.";
        return false;
    }

    if (isDuplicateContent(content)) {
        message = "Duplicate document content already exists.";
        return false;
    }
    int id = nextId;
    while (true) {
        string path = managedPathForId(id);
        error_code existsEc;
        if (!fs::exists(path, existsEc)) {
            break;
        }
        if (id == numeric_limits<int>::max()) {
            message = "No available document ID left.";
            return false;
        }
        ++id;
    }
    string finalPath = managedPathForId(id);
    string tempPath = finalPath + ".tmp";
    error_code copyEc;
    fs::copy_file(source, tempPath,
                  fs::copy_options::overwrite_existing, copyEc);
    if (copyEc) {
        message = "Could not copy file into managed folder: " + copyEc.message();
        return false;
    }
    error_code renameEc;
    fs::rename(tempPath, finalPath, renameEc);
    if (renameEc) {
        error_code removeEc;
        fs::remove(tempPath, removeEc);
        message = "Could not finalize copied file: " + renameEc.message();
        return false;
    }
    Document doc;
    doc.id = id;
    doc.title = source.filename().string();
    doc.content = content;

    documents.push_back(doc);
    pendingStatus.push_back(true);
    nextId = id + 1;

    string saveMessage;
    if (!saveCatalogue(saveMessage)) {
        documents.pop_back();
        pendingStatus.pop_back();

        error_code removeEc;
        fs::remove(finalPath, removeEc);
        message = "Import failed while saving catalogue: " + saveMessage;
        return false;
    }

    rebuildQueue();
    newId = id;
    message = "Document imported successfully.";
    return true;
}
void DocumentManager::listDocuments() const {
    if (documents.empty()) {
        cout << "No documents stored.\n";
        return;
    }
    cout << "ID | Pending | Size | Title\n";
    cout << "----------------------------------------\n";

    for (size_t i = 0; i < documents.size(); ++i) {
        cout << documents[i].id
                  << " | "
                  << (pendingStatus[i] ? "yes" : "no")
                  << " | "
                  << documents[i].content.size()
                  << " | "
                  << documents[i].title
                  << "\n";
    }
}

bool DocumentManager::getDocument(int id, Document &outDocument) const {
    for (const Document &doc : documents) {
        if (doc.id == id) {
            outDocument = doc;
            return true;
        }
    }
    return false;
}


bool DocumentManager::getNextPendingId(int &id) const {
    if (pendingQueue.empty()) {
        return false;
    }
    id = pendingQueue.front();
    return true;
}

bool DocumentManager::getNextPendingDocument(Document &outDocument) const {
    if (pendingQueue.empty()) {
        return false;
    }
    return getDocument(pendingQueue.front(), outDocument);
}

bool DocumentManager::markIndexed(int id, string &message) {
    int index = -1;
    for (size_t i = 0; i < documents.size(); ++i) {
        if (documents[i].id == id) {
            index = static_cast<int>(i);
            break;
        }
    }

    if (index < 0) {
        message = "Invalid document ID.";
        return false;
    }

    if (!pendingStatus[index]) {
        message = "Document is already marked as indexed.";
        return true;
    }

    pendingStatus[index] = false;

    string saveMessage;
    if (!saveCatalogue(saveMessage)) {
        pendingStatus[index] = true; // revert
        message = "Could not save indexing state: " + saveMessage;
        return false;
    }

    rebuildQueue();
    message = "Document marked as indexed.";
    return true;
}

size_t DocumentManager::pendingCount() const {
    return pendingQueue.size();
}

size_t DocumentManager::documentCount() const {
    return documents.size();
}
bool DocumentManager::saveCatalogue(string &message) {
    string tempFile = storageFile + ".tmp";
    error_code preRemoveEc;
    fs::remove(tempFile, preRemoveEc);
    ofstream out(tempFile, ios::binary | ios::trunc);
    if (!out) {
        message = "Could not open temporary catalogue file for writing.";
        return false;
    }
    out << "NEXTID " << nextId << "\n";
    out << "COUNT " << documents.size() << "\n";

    for (size_t i = 0; i < documents.size(); ++i) {
        const Document &doc = documents[i];
        bool pending = pendingStatus[i];
        out << "DOC\n";
        out << "ID " << doc.id << "\n";
        out << "PENDING " << (pending ? 1 : 0) << "\n";
        out << "TITLE " << doc.title << "\n";
        out << "SIZE " << static_cast<long long>(doc.content.size()) << "\n";
        out.write(doc.content.data(),
                  static_cast<streamsize>(doc.content.size()));

        out << "\nEND\n";
    }

    out.flush();
    if (!out) {
        message = "Failed while writing catalogue data.";
        out.close();
        error_code removeEc;
        fs::remove(tempFile, removeEc);
        return false;
    }

    out.close();
    if (!out) {
        message = "Failed while closing catalogue file.";
        error_code removeEc;
        fs::remove(tempFile, removeEc);
        return false;
    }
    error_code ec;
    if (fs::exists(storageFile, ec)) {
        error_code removeEc;
        fs::remove(storageFile, removeEc);
        if (removeEc) {
            message = "Could not replace old catalogue file.";
            return false;
        }
    }

    error_code renameEc;
    fs::rename(tempFile, storageFile, renameEc);
    if (renameEc) {
        message = "Could not move temporary catalogue into place: "
                  + renameEc.message();
        return false;
    }

    return true;
}

void DocumentManager::rebuildQueue() {
    while (!pendingQueue.empty()) {
        pendingQueue.pop();
    }
    for (size_t i = 0; i < documents.size(); ++i) {
        if (pendingStatus[i]) {
            pendingQueue.push(documents[i].id);
        }
    }
}
bool DocumentManager::isDuplicateContent(const string &content) const {
    for (const Document &doc : documents) {
        if (doc.content == content) {
            return true;
        }
    }
    return false;
}

string DocumentManager::managedPathForId(int id) const {
    fs::path p = fs::path(managedFolder) /
                 ("doc_" + to_string(id) + ".txt");
    return p.string();
}
