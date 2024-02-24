#include<fstream>

#include "blob.h"
#include "Utils.h"

using std::ofstream;
using std::ifstream;

Blob::Blob(const string& srcFile){
    this->srcFile = srcFile;
    this->content = readContents(srcFile);
    this->id = SHA1::sha1(srcFile, toString(content));
}

string Blob::getId() const{
    return id;
}

string Blob::getSrcfile() const{
    return srcFile;
}

void Blob::writeBlob(const string& file) const{
    ofstream fout(file);
    fout << "Blob\n";
    fout << srcFile << '\n';
    fout << id << '\n';
    for(auto line: content){
        fout << line << '\n';
    }
    fout.close();
}

bool Blob::read(const string& file){
    ifstream fin(file);
    string line;
    getline(fin, line);
    if(line != "Blob"){
        fin.close();
        return false;
    }
    getline(fin, srcFile);
    getline(fin, id);
    content.clear();
    while(getline(fin, line)){
        content.push_back(line);
    }
    fin.close();
    return true;
}

void Blob::writeContent() const{
    writeContents(srcFile, content);
}

vector<string> Blob::getContent() const{
    return content;
}