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
    //property
    JSDescription *jsDescription;
    string cipherTextWithABE;

    //con/destruct
    myTransaction(JSDescription *, string);
    ~myTransaction();
    
    //method
    size_t getTransactionHash() const;

    //class method
    static JSDescription makeSproutProof(
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

JSDescription myTransaction::makeSproutProof(
        const std::array<JSInput, 2>& inputs,
        const std::array<JSOutput, 2>& outputs,
        const Ed25519VerificationKey& joinSplitPubKey,
        uint64_t vpub_old,
        uint64_t vpub_new,
        const uint256& rt
){
    return JSDescription(joinSplitPubKey, rt, inputs, outputs, vpub_old, vpub_new);
}

bool myTransaction::verifySproutProof(
        const JSDescription& jsdesc,
        const Ed25519VerificationKey& joinSplitPubKey
)
{
    auto verifier = ProofVerifier::Strict();
    return verifier.VerifySprout(jsdesc, joinSplitPubKey);
}
