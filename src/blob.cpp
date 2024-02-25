#include<fstream>

#include "blob.h"
#include "Utils.h"

using std::ofstream;
using std::ifstream;

Blob::Blob(const string& srcFile){
    this->srcFile = srcFile;
    this->content = readContentsForBlob(srcFile);
    if(content.size() > 0 && content[content.size() - 1].empty()){
        content.pop_back();
        endWithNewLine = 1;
    }else{
        endWithNewLine = 0;
    }
    this->id = SHA1::sha1(srcFile, toString(content) + std::to_string(endWithNewLine));
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
    fout << endWithNewLine << '\n';
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
    fin >> endWithNewLine; fin.get();
    content.clear();
    while(getline(fin, line)){
        content.push_back(line);
    }
    fin.close();
    return true;
}

void Blob::writeContent() const{
    ofstream fout(srcFile);
    for(int i=0;i<content.size();i++){
        fout << content[i];
        if(i != content.size() - 1 || endWithNewLine){
            fout << '\n';
        }
    }
    fout.close();
}

vector<string> Blob::getContent() const{
    return content;
}

string Blob::getContentAsString() const{
    string res;
    for(int i=0;i<content.size();i++){
        res += content[i];
        if(i != content.size() - 1 || endWithNewLine){
            res += '\n';
        }
    }
    return res;
}

vector<string> Blob::readContentsForBlob(const string& file){
    vector<string> content;
    ifstream fin(file);
    string line;
    // 这样读取可以知道末尾是不是有空行，保证了读取内容一致
    while(!fin.eof()){
        getline(fin, line);
        content.push_back(line);
    }
    fin.close();
    return content;
}