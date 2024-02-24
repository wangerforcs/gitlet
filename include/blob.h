#ifndef _BLOB_H_
#define _BLOB_H_

#include<string>
#include<vector>

#include "sha1.h"
#include "Utils.h"

using std::string;
using std::vector;
class Blob{
    private:
    string id;
    string srcFile;
    vector<string> content;
    public:
    Blob(){};
    Blob(const string& srcFile);
    string getId() const;
    string getSrcfile() const;
    void writeBlob(const string& file) const;
    void writeContent() const;
    bool read(const string& file);
    vector<string> getContent() const;
};

#endif