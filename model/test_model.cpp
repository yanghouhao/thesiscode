#include <iostream>
#include <string>
#include <cassert>
#include <openabe/openabe.h>
#include <openabe/zsymcrypto.h>
#include "utilstrencodings.h"

#include <boost/foreach.hpp>
#include <boost/variant/get.hpp>

#include "zcash/prf.h"
#include "util.h"
#include "streams.h"
#include "version.h"
#include "serialize.h"
#include "primitives/transaction.h"
#include "proof_verifier.h"
#include "zcash/JoinSplit.hpp"
#include "zcash/Note.hpp"
#include "zcash/NoteEncryption.hpp"
#include "zcash/IncrementalMerkleTree.hpp"
#include <array>

#include <rust/ed25519/types.h>

#include "myBlock.h"
#include "myTransaction.h"

using namespace libzcash;
using namespace oabe;
using namespace oabe::crypto;
using namespace std;

int main ()
{
    return 0;
}

void testTransaction()
{

}

void testBlock()
{
    
}