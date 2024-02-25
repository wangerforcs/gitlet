#include "sys/types.h"
#include "sys/stat.h"
#include "unistd.h"
#include <iostream>
#include <queue>

#include "repo.h"
#include "Utils.h"
#include "commit.h"
#include "stage.h"

using std::cout;
using std::queue;

const string repo::CWD = "";
const string repo::GIT_DIR = ".gitlet";
const string repo::HEAD = join(GIT_DIR, "HEAD");
const string repo::STAGE = join(GIT_DIR, "stage");
const string repo::OBJECTS_DIR = join(GIT_DIR, "objects");
const string repo::REFS_DIR = join(GIT_DIR, "refs");
const string repo::HEADS_DIR = join(REFS_DIR, "heads");


void repo::init() {
    if (access(GIT_DIR.c_str(), 0) != -1) {
        Exit("A Gitlet version-control system already exists in the current directory.");
    }
    mkdir(GIT_DIR.c_str(), S_IRWXU);
    mkdir(OBJECTS_DIR.c_str(), S_IRWXU);
    mkdir(REFS_DIR.c_str(), S_IRWXU);
    mkdir(HEADS_DIR.c_str(), S_IRWXU);
    // 写入HEAD文件，作为当前分支
    writeContents(HEAD, "master");
    // initial commit
    Commit cmt;
    cmt.write(join(OBJECTS_DIR, cmt.getId()));
    writeContents(join(HEADS_DIR, "master"), cmt.getId());
}

void repo::add(const string& file){
    if(access(file.c_str(), 0) == -1){
        Exit("File does not exist.");
    }
    Stage stage;
    stage.read(repo::STAGE);
    if(stage.add(file)){
        stage.write(repo::STAGE);
    }
}

void repo::remove(const string& file){
    Stage stage;
    stage.read(repo::STAGE);
    if(stage.remove(file)){
        stage.write(repo::STAGE);
    }else{
        Exit("No reason to remove the file.");
    }
}

void repo::commit(const string& message){
    if(message.empty()){
        Exit("Please enter a commit message.");
    }
    Stage stage;
    stage.read(repo::STAGE);
    if(stage.empty()){
        Exit("No changes added to the commit.");
    }
    Commit commit = getCurCommit();
    auto blob = commit.getBlobs();
    auto ad = stage.getAddentry();
    auto rm = stage.getRmentry();
    for(auto i : ad){
        blob[i.first] = i.second;
    }
    for(auto i : rm){
        blob.erase(i);
    }
    decltype(commit.getParents()) parent;
    parent.push_back(commit.getId());
    Commit newCommit(message, parent, blob);
    newCommit.write(join(OBJECTS_DIR, newCommit.getId()));

    stage.reset();
    fllushHead(newCommit.getId());
}

void repo::log(){
    Commit commit = getCurCommit();
    while(true){
        commit.printlog();
        if(commit.isInit()){
            break;
        }
        commit = getParentCommits(commit)[0];
    }
}

void repo::globalLog(){
    auto commitlist = plainFilenamesIn(OBJECTS_DIR);
    Commit cmt;
    for(auto i : commitlist){
        if(cmt.read(join(OBJECTS_DIR, i))){
            cmt.printlog();
        }
    }
}

void repo::find(const string& message){
    auto commitlist = plainFilenamesIn(OBJECTS_DIR);
    Commit cmt;
    bool flag = false;
    for(auto i : commitlist){
        if(cmt.read(join(OBJECTS_DIR, i))){
            if(cmt.getMessage() == message){
                cout << i << '\n';
                flag = true;
            }
        }
    }
    if(!flag){
        Exit("Found no commit with that message.");
    }
}

void repo::status(){
    cout << "=== Branches ===\n";
    auto branchlist = plainFilenamesIn(HEADS_DIR);
    string branch = readContentsAsString(HEAD);
    for(auto i : branchlist){
        if(i == branch){
            cout << "*";
        }
        cout << i << '\n';
    }

    cout << "\n=== Staged Files ===\n";
    Stage stage;
    stage.read(repo::STAGE);
    auto addentry = stage.getAddentry();
    for(auto i : addentry){
        cout << getFileName(i.first) << '\n';
    }

    cout << "\n=== Removed Files ===\n";
    auto rmentry = stage.getRmentry();
    for(auto i : rmentry){
        cout << i << '\n';
    }

    cout << "\n=== Modifications Not Staged For Commit ===\n";
    // auto blobs = getCurCommit().getBlobs();
    // for(auto i : blobs){
    //     if(access(i.first.c_str(), 0) == -1){
    //         cout << i.first << " (deleted)\n";
    //         continue;
    //     }
    //     Blob blob(i.first);
    //     if(blob.getId() != i.second && !stage.getAddentry().count(i.first)){
    //         cout << i.first << " (modified)\n";
    //     }
    // }
    
    cout << "\n=== Untracked Files ===\n";
    
    // auto untracked = plainFilenamesIn(".");
    // Commit cmt = getCurCommit();
    // for(auto i : untracked){
    //     if(i != GIT_DIR && !cmt.Commited(i)){
    //         cout << i << '\n';
    //     }
    // }
}

void repo::checkout(const string& file){
    Commit cmt = getCurCommit();
    if(cmt.Commited(file)){
        string blobid = cmt.getBlobs()[file]; 
        Blob blob;
        if(blob.read(join(OBJECTS_DIR, blobid))){
            blob.writeContent();
        }
    }else{
        Exit("File does not exist in that commit.");
    }
}

void repo::checkout(const string& commitId, const string& file){
    string fullId = commitId;
    fillCommitId(fullId);
    Commit cmt = getCommit(fullId);
    if(cmt.Commited(file)){
        string blobid = cmt.getBlobs()[file]; 
        Blob blob;
        if(blob.read(join(OBJECTS_DIR, blobid))){
            blob.writeContent();
        }
    }else{
        Exit("File does not exist in that commit.");
    }
}

void repo::checkoutBranch(const string& branch){
    string curBranch = readContentsAsString(HEAD);
    if(curBranch == branch){
        Exit("No need to checkout the current branch.");
    }

    string branchFile = join(HEADS_DIR, branch);
    if(access(branchFile.c_str(), 0) == -1){
        Exit("No such branch exists.");
    }

    string commitId = readContentsAsString(branchFile);
    changeCommit(commitId);

    setBranch(branch);
}

void repo::branch(const string& branch){
    auto branchlist = plainFilenamesIn(HEADS_DIR);
    for(auto i : branchlist){
        if(i == branch){
            Exit("A branch with that name already exists.");
        }
    }

    Commit cmt = getCurCommit();
    writeContents(join(HEADS_DIR, branch), cmt.getId());
}

void repo::rmBranch(const string& branch){
    auto curBranch = getBranch();
    if(curBranch == branch){
        Exit("Cannot remove the current branch.");
    }
    auto branchlist = plainFilenamesIn(HEADS_DIR);
    bool flag = false;
    for(auto i : branchlist){
        if(i == branch){
            std::remove(join(HEADS_DIR, branch).c_str());
            flag = true;
            break;
        }   
    }
    if(!flag){
        Exit("A branch with that name does not exist.");
    }
}

void repo::reset(const string& commitId){
    string fullId = commitId;
    fillCommitId(fullId);
    changeCommit(fullId);
    fllushHead(fullId);
}

void repo::merge(const string& branch){
    string curBranch = getBranch();
    if(curBranch == branch){
        Exit("Cannot merge a branch with itself.");
    }
    if(access(join(HEADS_DIR, branch).c_str(), 0) == -1){
        Exit("A branch with that name does not exist.");
    }
    Stage stage;
    stage.read(repo::STAGE);
    if(!stage.empty()){
        Exit("You have uncommitted changes.");
    }
    bool confilicted = false;
    Commit curCmt = getCurCommit();
    Commit givenCmt = getHeadCommitByBranch(branch);
    Commit ancester = getAncesterCommit(curCmt, givenCmt);
    if(ancester.getId() == givenCmt.getId()){
        Exit("Given branch is an ancestor of the current branch.");
    }
    if(ancester.getId() == curCmt.getId()){
        checkoutBranch(branch);
        Exit("Current branch fast-forwarded.");
    }

    auto curBlob = curCmt.getBlobs();
    auto givenBlob = givenCmt.getBlobs();
    auto ancesterBlob = ancester.getBlobs();
    auto mergeBlob = curBlob;

    for(auto i : givenBlob){
        if(!curBlob.count(i.first) && !ancesterBlob.count(i.first)){
            if(access(i.first.c_str(), 0) != -1)
                Exit("There is an untracked file in the way; delete it, or add and commit it first.");
        }
    }

    for(auto i : givenBlob){
        if(curBlob.count(i.first)){
            if(ancesterBlob.count(i.first)){
                if(ancesterBlob[i.first] == curBlob[i.first]){
                    // 1 当前和祖先相同，和给定不同
                    if(i.second != curBlob[i.first]){
                        checkout(givenCmt.getId(), i.first);
                        mergeBlob[i.first] = i.second;
                        stage.add(i.first);
                    }
                }else{
                    // 8 均不同
                    if(i.second != curBlob[i.first] && i.second != ancesterBlob[i.first]){
                        confilicted = true;
                        mergeBlob[i.first] = dealConflict(i.first, curBlob[i.first], i.second);
                    }

                }
            }else{
                // 8 祖先不追踪，当前和给定不同
                if(i.second != curBlob[i.first]){
                    confilicted = true;
                    mergeBlob[i.first] = dealConflict(i.first, curBlob[i.first], i.second);
                }
            }
        }else{
            if(ancesterBlob.count(i.first)){
                // 8 当前不追踪，且给定和祖先不同
                if(ancesterBlob[i.first] != i.second){
                    confilicted = true;
                    mergeBlob[i.first] = dealConflict(i.first, "", i.second);
                }
            } else{
                // 5 当前不追踪，祖先不追踪，只有给定追踪
                // Blob blob;
                // blob.read(join(OBJECTS_DIR, i.second));
                // blob.writeContent();
                checkout(givenCmt.getId(), i.first);
                mergeBlob[i.first] = i.second;
                stage.add(i.first);
            }
        }
    }
    for(auto i : curBlob){
        if(!givenBlob.count(i.first)){
            if(ancesterBlob.count(i.first)){
                // 6 给定不追踪，当前和祖先相同
                if(ancesterBlob[i.first] == i.second){
                    std::remove(i.first.c_str());
                    mergeBlob.erase(i.first);
                }else{
                // 8 给定不追踪，当前和祖先不同
                    confilicted = true;
                    mergeBlob[i.first] = dealConflict(i.first, i.second, "");
                }
            }
        }
    }

    string message = "Merged " + branch + " into " + curBranch + ".";
    Commit commit(message, {curCmt.getId(), givenCmt.getId()}, mergeBlob);
    commit.write(join(OBJECTS_DIR, commit.getId()));
    fllushHead(commit.getId());
    stage.reset();

    if(confilicted){
        std::cout << "Encountered a merge conflict.\n";
    }
}

void repo::checkRepo(){
    if(access(repo::GIT_DIR.c_str(), 0) == -1){
        Exit("Not in an initialized Gitlet directory.");
    }
}

Commit repo::getCurCommit(){
    string branch = readContentsAsString(HEAD);
    return getHeadCommitByBranch(branch);
}

vector<Commit> repo::getParentCommits(const Commit& cmt){
    vector<Commit> res;
    for(auto i : cmt.getParents()){
        res.push_back(getCommit(i));
    }
    return res;
}

Commit repo::getCommit(const string& commitId){
    string file = join(OBJECTS_DIR, commitId);
    if(access(file.c_str(), 0) == -1){
        Exit("No commit with that id exists.");
    }
    Commit cmt;
    cmt.read(join(OBJECTS_DIR, commitId));
    return cmt;
}

Commit repo::getHeadCommitByBranch(const string& branch){
    string commitId = readContentsAsString(join(HEADS_DIR, branch));
    Commit cmt;
    cmt.read(join(OBJECTS_DIR, commitId));
    return cmt;
}

string repo::getBranch(){
    return readContentsAsString(HEAD);
}

void repo::setBranch(const string& branch){
    writeContents(HEAD, branch);
}

void repo::resetStage(){
    Stage stage;
    stage.reset();
}

void repo::changeCommit(const string& commitId){
    Commit cmt = getCommit(commitId);
    Commit curCmt = getCurCommit();
    auto curBlob = curCmt.getBlobs();
    auto newBlob = cmt.getBlobs();

    // 在checkout commit，不在当前commit， 存在于工作目录
    for(auto i : newBlob){
        if(!curBlob.count(i.first)){
            if(access(i.first.c_str(), 0) != -1){
                Exit("There is an untracked file in the way; delete it, or add and commit it first.");
            }
        }
    }

    // 在当前commit，不在checkout commit
    for(auto i : curBlob){
        if(newBlob.count(i.first) == 0){
            std::remove(i.first.c_str());
        }
    }

    // 在checkout commit，也在当前commit
    // 或者在checkout commit，不在当前commit， 不存在于工作目录
    // 均需要重新写入文件
    for(auto i : newBlob){
        Blob blob;
        if(blob.read(join(OBJECTS_DIR, i.second))){
            blob.writeContent();
        }
    }

    resetStage();
}

void repo::fllushHead(const string& commitId){
    string branch = readContentsAsString(HEAD);
    writeContents(join(HEADS_DIR, branch), commitId);
}

void repo::fillCommitId(string& commitId){
    auto commitlist = plainFilenamesIn(OBJECTS_DIR);
    for(auto i : commitlist){
        if(startsWith(i, commitId)){
            commitId = i;
            break;  
        }
    }
}

Commit repo::getAncesterCommit(const Commit& cmt, const Commit& givenCmt){
    queue<Commit> q;
    q.push(cmt);
    while(!q.empty()){
        Commit cur = q.front();
        q.pop();
        if(isAncester(cur, givenCmt)){
            return cur;
        }
        for(auto i : getParentCommits(cur)){
            q.push(i);
        }
    }
}

bool repo::isAncester(const Commit& cmt, const Commit& givenCmt){
    Commit parent = givenCmt;
    if(parent.getId() == cmt.getId()){
        return true;
    }
    if(parent.isInit()){
        return false;
    }
    for(auto i : getParentCommits(parent)){
        if(isAncester(cmt, i)){
            return true;
        }
    }
    return false;
}

string repo::dealConflict(const string& file, const string& blob1, const string& blob2){
    Blob blob;
    string content1, content2;
    if(blob1 != ""){
        blob.read(join(OBJECTS_DIR, blob1));
        content1 = blob.getContentAsString();
    }
    if(blob2 != ""){
        blob.read(join(OBJECTS_DIR, blob2));
        content2 = blob.getContentAsString();
    }
    string newContent = "<<<<<<< HEAD\n" + content1 + "=======\n" + content2 + ">>>>>>>\n";

    writeContents(file, newContent);
    Blob newBlob(file);
    newBlob.writeBlob(join(OBJECTS_DIR, newBlob.getId()));
    return newBlob.getId();
}
