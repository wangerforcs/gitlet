#include "stage.h"
#include "repo.h"
#include "commit.h"
#include "unistd.h"
#include <fstream>

using std::ofstream;
using std::ifstream;

void Stage::write(const string& file){
    ofstream fout(file);
    fout << addentry.size() << '\n';
    for(auto i : addentry){
        fout << i.first << '\n' << i.second << '\n';
    }
    fout << rmentry.size() << '\n';
    for(auto i : rmentry){
        fout << i << '\n';
    }
    fout.close();
}

void Stage::read(const string& file){
    ifstream fin(file);
    addentry.clear();
    rmentry.clear();
    if(!fin.is_open()){
        return;
    }
    int n;
    string filename;
    string blob;
    fin >> n; fin.get();
    for(int i=0;i<n;i++){
        getline(fin, filename);
        getline(fin, blob);
        addentry.insert({filename, blob});
    }
    fin >> n; fin.get();
    for(int i=0;i<n;i++){
        getline(fin, filename);
        rmentry.insert(filename);
    }
    fin.close();
}

bool Stage::add(const string& file){
    Blob blob(file);
    if(addentry.count(file) > 0){
        if(addentry[file] == blob.getId()){
            return false;
        }
    }
    rmentry.erase(file);
    Commit commit = repo::getCurCommit();
    if(!commit.Commited(blob)){
        addentry[file] = blob.getId();
        blob.writeBlob(join(repo::OBJECTS_DIR, blob.getId()));
    }else{
        addentry.erase(file);
    }
    return true;
}

bool Stage::remove(const string& file){
    if(addentry.count(file) > 0){
        addentry.erase(file);
        return true;
    }
    
    Commit commit = repo::getCurCommit();
    if(commit.Commited(file)){
        if(access(file.c_str(), 0) != -1){
            std::remove(file.c_str());
        }
        rmentry.insert(file);
        return true;
    }
    return false;
}

bool Stage::empty() const{
    return addentry.empty() && rmentry.empty();
}

unordered_map<string, string> Stage::getAddentry() const{
    return addentry;
}

unordered_set<string> Stage::getRmentry() const{
    return rmentry;
}

void Stage::reset() {
    addentry.clear();
    rmentry.clear();
    write(repo::STAGE);
}