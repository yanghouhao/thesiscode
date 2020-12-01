#include "primitives/transaction.h"
#include "proof_verifier.h"

using namespace libzcash;
using namespace std;

class MyTransaction
{
private:
    /* data */
    size_t transactionHash;
public:
    //property
    JSDescription *jsDescription;
    string cipherTextWithABE;

    //con/destruct
    MyTransaction(JSDescription *, string);
    MyTransaction();
    ~MyTransaction();
    
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
