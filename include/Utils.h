#ifndef _UTILS_H_
#define _UTILS_H_

#include <string>
#include <vector>
#include <unordered_map>

using std::string;
using std::vector;
using std::unordered_map;

vector<string> readContents(const string& file);

void writeContents(const string& file, const string& line);

void writeContents(const string& file, const vector<string>& content);

string join(const string& dir, const string& file);

string getParentFile(const string& file);

void Exit(const string& msg);

string toString(const vector<string>& lines);

string toString(const unordered_map<string, string>& umap);

string toStringSep(const vector<string>& lines, const string& sep);

string readContentsAsString(const string& file);

vector<string> plainFilenamesIn(const string& dir);

bool startsWith(const string& str, const string& head);

string getFileName(const string& file);

#endif