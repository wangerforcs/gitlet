#ifndef _COMMIT_H_
#define _COMMIT_H_

#include<string>
#include<vector>
#include<unordered_map>

#include "blob.h"

using std::string;
using std::vector;
using std::unordered_map;

class Commit{
private:
    string id;
    string timestamp;
    string message;
    vector<string> parents;
    unordered_map<string, string> blobs;
public:
    Commit();
    Commit(const string& message, const vector<string>& parents, const unordered_map<string, string>& blobs);
    string Time();
    string originalTime();
    bool read(const string& file);
    void write(const string& file);
    string getId() const;
    string getMessage() const;
    vector<string> getParents() const;
    unordered_map<string, string> getBlobs() const;
    void printlog() const;
    bool Commited(const Blob& blob) const;
    bool Commited(const string& file) const;
    bool isInit() const;
};


#endif