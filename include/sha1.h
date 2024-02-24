#ifndef _SHA1_H_
#define _SHA1_H_

#include<vector>
#include<string>

using std::string;
using std::vector;

namespace SHA1{
    class SHA{
        private:
        typedef uint8_t BYTE;
        typedef uint32_t WORD;
        WORD A,B,C,D,E;
        vector<WORD> Word;
        void reset();
        string padding(string message);
        inline WORD charToWord(char ch);
        WORD shiftLeft(WORD x, int n);
        WORD kt(int t);
        WORD ft(int t, WORD B, WORD C, WORD D);
        void getWord(string& message, int index);
        public:
        SHA();
        string sha(string message);
    };
    string sha1(string message);
    string sha1(string s1, string s2);
    string sha1(string s1, string s2, string s3, string s4);
}

#endif