#include <iostream>
#include <unordered_map>
#include <string>

#include "repo.h"
#include "Utils.h"

using namespace std;

int main(int argc, char* argv[]){
    if(argc < 2){
        Exit("Please enter a command.");
    }
    string cmd = argv[1];
    if(cmd == "init"){
        repo::init();
    }else if(cmd == "add"){
        if(argc < 3){
            Exit("Please enter a file to add.");
        }
        repo::checkRepo();
        repo::add(argv[2]);
    }else if(cmd == "commit"){
        if(argc < 3){
            Exit("Please enter a commit message.");
        }
        repo::checkRepo();
        repo::commit(argv[2]);
    }else if(cmd == "rm"){
        if(argc < 3){
            Exit("Please enter a file to remove.");
        }
        repo::checkRepo();
        repo::remove(argv[2]);
    }else if(cmd == "log"){
        repo::checkRepo();
        repo::log();
    }else if(cmd == "global-log"){
        repo::checkRepo();
        repo::globalLog();
    }else if(cmd == "find"){
        if(argc < 3){
            Exit("Please enter a commit message.");
        }
        repo::checkRepo();
        repo::find(argv[2]);
    }else if(cmd == "status"){
        repo::checkRepo();
        repo::status();
    }else if(cmd == "checkout"){
        if(argc < 3){
            Exit("Please enter a file or commit id.");
        }
        repo::checkRepo();
        if(argc == 3){
            repo::checkoutBranch(argv[2]);
        }else if(argc == 4){
            string op = argv[2];
            if(op != "--"){
                Exit("Incorrect operands.");
            }
            repo::checkout(argv[3]);
        }else if(argc == 5){
            string op = argv[3];
            if(op != "--"){
                Exit("Incorrect operands.");
            }
            repo::checkout(argv[2], argv[4]);
        }
    }else if(cmd == "branch"){
        if(argc < 3){
            Exit("Please enter a branch name.");
        }
        repo::checkRepo();
        repo::branch(argv[2]);
    }else if(cmd == "rm-branch"){
        if(argc < 3){
            Exit("Please enter a branch name.");
        }
        repo::checkRepo();
        repo::rmBranch(argv[2]);
    }else if(cmd == "reset"){
        if(argc < 3){
            Exit("Please enter a commit id.");
        }
        repo::checkRepo();
        repo::reset(argv[2]);
    }else if(cmd == "merge"){
        if(argc < 3){
            Exit("Please enter a branch name.");
        }
        repo::checkRepo();
        repo::merge(argv[2]);
    }else{
        Exit("No command with that name exists.");
    }
    
    return 0;
}
