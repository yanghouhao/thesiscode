#include "MyTransaction.h"

#include <openabe/openabe.h>
#include <openabe/zsymcrypto.h>

using namespace oabe;
using namespace oabe::crypto;

MyTransaction::MyTransaction(JSDescription *jsDescription, string cipherTextWithABE)
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

MyTransaction::~MyTransaction()
{
    delete this->jsDescription;
}

size_t MyTransaction::getTransactionHash() const
{
    return this->transactionHash;
}

JSDescription MyTransaction::makeSproutProof(
        const std::array<JSInput, 2>& inputs,
        const std::array<JSOutput, 2>& outputs,
        const Ed25519VerificationKey& joinSplitPubKey,
        uint64_t vpub_old,
        uint64_t vpub_new,
        const uint256& rt
){
    return JSDescription(joinSplitPubKey, rt, inputs, outputs, vpub_old, vpub_new);
}

bool MyTransaction::verifySproutProof(
        const JSDescription& jsdesc,
        const Ed25519VerificationKey& joinSplitPubKey
)
{
    auto verifier = ProofVerifier::Strict();
    return verifier.VerifySprout(jsdesc, joinSplitPubKey);
}