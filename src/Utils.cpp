#include <iostream>
#include <fstream>
#include <numeric>
#include <dirent.h>
#include <sys/types.h>
#include <cstring>

#include "Utils.h"

using std::ifstream;
using std::ofstream;

vector<string> readContents(const string& file) {
    vector<string> content;
    ifstream fin(file);
    string line;
    while (getline(fin, line)) {
        content.push_back(line);
    }
    fin.close();
    return content;
}

void writeContents(const string& file, const string& line){
    ofstream fout(file);
    fout << line;
    fout.close();
}

void writeContents(const string& file, const vector<string>& content){
    ofstream fout(file);
    for (auto line : content) {
        fout << line + '\n';
    }
    fout.close();
}
string join(const string& dir, const string& file){
    return dir + '/' + file;
}

string getParentFile(const string& file) {
    for (int i = file.size() - 1; i >= 0; i--) {
        if (file[i] == '/') {
            return file.substr(0, i);
        }
    }
    return "";
}

void Exit(const string& msg) {
    std::cout << msg << std::endl;
    exit(1);
}

string toString(const vector<string>& lines) {
    string text = "";
    return accumulate(lines.begin(), lines.end(), text);
}

string toString(const unordered_map<string, string>& umap) {
    string text = "";
    for (auto it : umap) {
        text += it.first + it.second;
    }
    return text;
}

string toStringSep(const vector<string>& lines, const string& sep){
    string text = "";
    for (auto line : lines) {
        text += line + sep;
    }
    return text;
}



string readContentsAsString(const string& file) {
    return toString(readContents(file));
}

vector<string> plainFilenamesIn(const string& dir) {
    vector<string> files;
    DIR* pDir;
    struct dirent* ptr;
    if (!(pDir = opendir(dir.c_str()))) {
        std::cout << "Folder doesn't exist!" << std::endl;
        exit(1);
    }
    while (ptr = readdir(pDir)) {
        if (strcmp(ptr->d_name, ".") && strcmp(ptr->d_name, "..")) {
            files.push_back(ptr->d_name);
        }
    }
    closedir(pDir);
    return files;
}

bool startsWith(const string& str, const string& head) {
    return str.compare(0, head.size(), head) == 0;
}

string getFileName(const string& file) {
    string name = "";
    for (int i = file.size() - 1; i >= 0; i--) {
        if (file[i] == '/') {
            break;
        }
        name = file[i] + name;
    }
    return name;
}
