#ifndef _AUDITCONTENT_H_
#define _AUDITCONTENT_H_

#include <openabe/openabe.h>
#include <openabe/zsymcrypto.h>

using namespace oabe;
using namespace oabe::crypto;
using namespace std;

class CommitmentContent
{
private:
    G_t V;
    ZP_t r;
public:
    CommitmentContent(){};
    CommitmentContent(ZP_t, ZP_t);
    ~CommitmentContent();
    ZP_t getChallenge();
    bool verify(G_t);
};

class CipherTextContent
{
private:
    string cipherText;
public:
    CipherTextContent(){};  
    CipherTextContent(string, string);//密钥在前，明文在后
    string decString(string);
};

class AuditContent
{
private:
    vector<string> cipherTextArrayWithABE;
    vector<string> apkArray;
    CommitmentContent keyCommitment;
    CipherTextContent keyCipherText;
public:
    AuditContent(){};
    AuditContent(vector<string> cipherTextArrayWithABE, vector<string> apkArray, CommitmentContent keyCommitment, CipherTextContent keyCipherText){
        this->cipherTextArrayWithABE = cipherTextArrayWithABE;
        this->apkArray = apkArray;
        this->keyCommitment = keyCommitment;
        this->keyCipherText = keyCipherText;
    };
    ~AuditContent();

    vector<string> getCipherTextArrayWithABE(){return this->cipherTextArrayWithABE;};
    vector<string> getApkArray(){return this->apkArray;};
    CommitmentContent getKeyCommitment(){return this->keyCommitment;};
    CipherTextContent getKeyCipherText(){return this->keyCipherText;};
};

#endif