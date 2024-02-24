#ifndef _STAGE_H_
#define _STAGE_H_

#include <string>
#include <unordered_map>
#include <unordered_set>

#include "blob.h"

using std::string;
using std::unordered_map;
using std::unordered_set;

class Stage{
private:
    unordered_map<string, string> addentry;
    unordered_set<string> rmentry;
public:
    void write(const string& file);
    void read(const string& file);
    bool add(const string& file);
    bool remove(const string& file);
    bool empty() const;
    void reset();
    unordered_map<string, string> getAddentry() const;
    unordered_set<string> getRmentry() const;
};

#endif