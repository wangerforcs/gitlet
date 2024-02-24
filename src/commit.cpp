#include <time.h>
#include <fstream>
#include <iostream>

#include "commit.h"
#include "sha1.h"
#include "Utils.h"

using std::ofstream;
using std::ifstream;
using std::cout;

Commit::Commit(){
    message = "initial commit";
    timestamp = originalTime();
    id = SHA1::sha1(timestamp, message);
}

Commit::Commit(const string& message, const vector<string>& parents, const unordered_map<string, string>& blobs){
    this->message = message;
    this->parents = parents;
    this->blobs = blobs;
    this->timestamp = Time();
    this->id = SHA1::sha1(timestamp, message, toString(parents), toString(blobs));
}

bool Commit::read(const string& file){
    blobs.clear();
    parents.clear();
    ifstream fin(file);
    string line;
    getline(fin, line);
    if(line != "Commit"){
        fin.close();
        return false;
    }
    getline(fin, id);
    getline(fin, timestamp);
    getline(fin, message);
    int n;
    fin >> n; fin.get();
    for(int i = 0; i < n; i++){
        getline(fin, line);
        parents.push_back(line);
    }
    fin >> n; fin.get();
    string blob;
    for(int i = 0; i < n; i++){
        getline(fin, line);
        getline(fin, blob);
        blobs[line] = blob;
    }
    fin.close();
    return true;
}

void Commit::write(const string& file){
    ofstream fout(file);
    fout << "Commit\n";
    fout << id << '\n';
    fout << timestamp;
    fout << message << '\n';
    fout << parents.size() << '\n';
    for(auto parent: parents){
        fout << parent << '\n';
    }
    fout << blobs.size() << '\n';
    for(auto blob: blobs){
        fout << blob.first << '\n' << blob.second << '\n';
    }
    fout.close();
}

void Commit::printlog() const {
    cout << "===\n";
    cout << "commit " << id << '\n';
    if(parents.size() > 1){
        cout << "Merge: ";
        for(auto parent: parents){
            cout << parent.substr(0, 7) << ' ';
        }
        cout << '\n';
    }
    cout << "Date: " << timestamp << '\n';
    cout << message << '\n';
    cout << '\n';
}

string Commit::getId() const {
    return id;
}

string Commit::getMessage() const {
    return message;
}

unordered_map<string, string> Commit::getBlobs() const{
    return blobs;
}

vector<string> Commit::getParents() const{
    return parents;
}

string Commit::Time(){
    time_t now = time(0);
    string date = ctime(&now);
    date = date.substr(0,date.size()-1);
    date += " -0800\n";
    return date;
}

string Commit::originalTime(){
    return "Thu Jan 1 00:00:00 1970 -0800\n";
}

bool Commit::Commited(const Blob& blob) const{
    string file = blob.getSrcfile();
    if(blobs.count(file) > 0){
        if(blobs.at(file) == blob.getId()){
            return true;
        }
    }
    return false;
}

bool Commit::Commited(const string& file) const{
    if(blobs.count(file) > 0){
        return true;
    }
    return false;
}

bool Commit::isInit() const {
    return parents.empty();
}