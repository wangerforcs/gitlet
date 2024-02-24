#include <sstream>
#include <iomanip>

#include "sha1.h"

using std::stringstream;

namespace SHA1{
    void SHA::reset(){
        A = 0x67452301;
        B = 0xEFCDAB89;
        C = 0x98BADCFE;
        D = 0x10325476;
        E = 0xC3D2E1F0;
    }
    string SHA::padding(string message){
        int originalLength = message.length();
        int newLength = ((originalLength + 8) + 63) / 64 * 64;
        string newMessage = message;
        newMessage.resize(newLength, 0);
        newMessage[originalLength] = 0x80;
        int bitLength = originalLength * 8;
        for(int i=newLength-1;i>=newLength-8;i--){
            newMessage[i] = bitLength % 256;
            bitLength /= 256;
        }
        return newMessage;
    }
    SHA::WORD SHA::charToWord(char ch){
        return (BYTE)ch;
    }
    SHA::WORD SHA::shiftLeft(WORD x, int n){
        return (x >> (32 - n)) | (x << n);
    }
    void SHA::getWord(string& message, int index){
        for(int i=0; i<16; i++){
            Word[i] = (charToWord(message[index + 4*i]) << 24) + (charToWord(message[index + 4*i + 1]) << 16) + 
            (charToWord(message[index + 4*i + 2]) << 8) + charToWord(message[index + 4*i + 3]);
        }
        for(int i=16; i<80; i++){
            Word[i] = shiftLeft(Word[i-3] ^ Word[i-8] ^ Word[i-14] ^ Word[i-16], 1);
        }
    }
    SHA::SHA():Word(80){
        reset();
    }
    SHA::WORD SHA::kt(int t){
        if (t < 20)
            return 0x5a827999;
        else if (t < 40)
            return 0x6ed9eba1;
        else if (t < 60)
            return 0x8f1bbcdc;
        else
            return 0xca62c1d6;
    }
    SHA::WORD SHA::ft(int t, WORD B, WORD C, WORD D){
        if (t < 20)
            return (B & C) | ((~B) & D);
        else if (t < 40)
            return B ^ C ^ D;
        else if (t < 60)
            return (B & C) | (B & D) | (C & D);
        else
            return B ^ C ^ D;
    }
    string SHA::sha(string message){
        reset();
        message = padding(message);
        int byteLength = message.length();
        for(int i=0; i < byteLength; i+=64){
            getWord(message, i);
            WORD a = A, b = B, c = C, d = D, e = E;
            for(int j=0; j<80; j++){
                WORD temp = shiftLeft(a, 5) + ft(j, b, c, d) + e + kt(j) + Word[j];
                e = d;
                d = c;
                c = shiftLeft(b, 30);
                b = a;
                a = temp;
            }
            A += a;
            B += b;
            C += c;
            D += d;
            E += e;

        }
        stringstream ss;
        ss << std::hex;
        ss << std::setw(8) << std::setfill('0');
        ss << A;
        ss << std::setw(8) << std::setfill('0');
        ss << B;
        ss << std::setw(8) << std::setfill('0');
        ss << C;
        ss << std::setw(8) << std::setfill('0');
        ss << D;
        ss << std::setw(8) << std::setfill('0');
        ss << E;
        return ss.str();
    }
    SHA sha;
    string sha1(string message){
        return sha.sha(message);
    }
    string sha1(string s1, string s2) {
        return sha1(s1 + s2);
    }

    string sha1(string s1, string s2, string s3, string s4) {
        return sha1(s1 + s2 + s3 + s4);
    }
};

