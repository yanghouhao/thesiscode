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
    transaction(JSDescription jsDescription,
    string cipherTextWithABE);
    ~transaction();
    JSDescription *jsDescription;
    string cipherTextWithABE;
    size_t getTransactionHash() const;
};

myTransaction::myTransaction(JSDescription *jsDescription,
    string cipherTextWithABE)
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
    this->transactionHash = 0;
}

myTransaction::~myTransaction()
{
    delete this->jsDescription;
}

myTransaction::getTransactionHash()
{
    if (this->transactionHash == 0)
    {
        this->transactionHash = std::hash<size_t>()(this->transactionHash);
    }
    return this->transactionHash;
}
