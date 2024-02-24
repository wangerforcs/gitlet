#ifndef _REPO_H_
#define _REPO_H_

#include <string>

#include "commit.h"

using std::string;
using std::vector;

class repo{
public:
    const static string CWD;
    const static string HEAD;
    const static string STAGE;
    const static string GIT_DIR;
    const static string OBJECTS_DIR;
    const static string REFS_DIR;
    const static string HEADS_DIR;

    static void init();
    static void add(const string& file);
    static void remove(const string& file);
    static void commit(const string& message);
    static void log();
    static void globalLog();
    static void find(const string& message);
    static void status();
    static void checkout(const string& file);
    static void checkout(const string& commitId, const string& file);
    static void checkoutBranch(const string& branch);
    static void branch(const string& branch);
    static void rmBranch(const string& branch);
    static void reset(const string& commitId);
    static void merge(const string& branch);
    static void checkRepo();
    static void fllushHead(const string& commitId);
    static Commit getCurCommit();
    static vector<Commit> getParentCommits(const Commit& cmt);
    static Commit getCommit(const string& commitId);
    static string getBranch();
    static void changeCommit(const string& commitId);
    static void setBranch(const string& branch);
    static void resetStage();
    static void fillCommitId(string& commitId);
    static Commit getHeadCommitByBranch(const string& branch);
    static Commit getAncesterCommit(const Commit& cmt, const Commit& givenCmt);
    static bool isAncester(const Commit& cmt, const Commit& givenCmt);
    static string dealConflict(const string& file, const string& blob1, const string& blob2);
};

#endif