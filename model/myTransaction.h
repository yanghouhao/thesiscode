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
