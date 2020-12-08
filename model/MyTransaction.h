#ifndef _MYTRANSACTION_H_
#define _MYTRANSACTION_H_

#include "primitives/transaction.h"
#include "proof_verifier.h"

using namespace libzcash;
using namespace std;

enum TransactionType {
    TransactionTypeTransfer,
    TransactionTypeTreate,
};

class MyTransaction
{
private:
    /* data */
    size_t transactionHash;
public:
    //property
    JSDescription *jsDescription;
    std::vector<string> cipherTextArrayWithABE;
    std::vector<string> apkArray;
    TransactionType type;

    //con/destruct
    MyTransaction(JSDescription *, std::vector<string>, TransactionType, std::vector<string>);
    MyTransaction();
    ~MyTransaction();
    
    //method
    size_t getTransactionHash() const;

    //class method
    static JSDescription * makeSproutProof(
        const std::array<JSInput, 2>&,
        const std::array<JSOutput, 2>&,
        const Ed25519VerificationKey&,
        uint64_t,
        uint64_t,
        const uint256&);

    static bool verifySproutProof(
        const JSDescription&,
        const Ed25519VerificationKey&);
};

#endif