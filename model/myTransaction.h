#include "primitives/transaction.h"


using namespace libzcash;
using namespace oabe;
using namespace oabe::crypto;
using namespace std;

class myTransaction
{
private:
    /* data */
    size_t transactionHash;
public:
    myTransaction(JSDescription *, string);
    ~myTransaction();
    JSDescription *jsDescription;
    string cipherTextWithABE;
    size_t getTransactionHash() const;
};

myTransaction::myTransaction(JSDescription *jsDescription, string cipherTextWithABE)
{
    this->jsDescription = new JSDescription;
    this->jsDescription->vpub_old = jsDescription->vpub_old ;
    this->jsDescription->vpub_new = jsDescription->vpub_new ;
    this->jsDescription->anchor = jsDescription->anchor ;
    this->jsDescription->nullifiers = jsDescription->nullifiers ;
    this->jsDescription->commitments = jsDescription->commitments ;
    this->jsDescription->ephemeralKey = jsDescription->ephemeralKey ;
    this->jsDescription->ciphertexts = jsDescription->ciphertexts ;
    this->jsDescription->randomSeed = jsDescription->randomSeed ;
    this->jsDescription->macs = jsDescription->macs ;
    this->jsDescription->proof = jsDescription->proof;

    this->cipherTextWithABE = cipherTextWithABE;
    this->transactionHash = std::hash<string>()(this->cipherTextWithABE);
}

myTransaction::~myTransaction()
{
    delete this->jsDescription;
}

size_t myTransaction::getTransactionHash() const
{
    return this->transactionHash;
}
