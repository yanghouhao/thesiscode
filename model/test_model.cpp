#include <string>
#include <cassert>
#include <openabe/openabe.h>
#include <openabe/zsymcrypto.h>

#include <boost/variant/get.hpp>

#include "zcash/prf.h"
#include "version.h"
#include "primitives/transaction.h"
#include "proof_verifier.h"

#include "zcash/Note.hpp"
#include "zcash/NoteEncryption.hpp"
#include "zcash/IncrementalMerkleTree.hpp"
#include <array>
#include "clientversion.h"
#include "uint256.h"
#include "zcash/Address.hpp"

#include <rust/ed25519/types.h>

#include "random.h"
#include "librustzcash.h"


#include "test/data/merkle_roots.json.h"
#include "test/data/merkle_serialization.json.h"
#include "test/data/merkle_witness_serialization.json.h"
#include "test/data/merkle_path.json.h"
#include "test/data/merkle_commitments.json.h"

#include "test/data/merkle_roots_sapling.json.h"
#include "test/data/merkle_serialization_sapling.json.h"
#include "test/data/merkle_witness_serialization_sapling.json.h"
#include "test/data/merkle_path_sapling.json.h"
#include "test/data/merkle_commitments_sapling.json.h"

#include <iostream>

#include <stdexcept>



#include "zcash/util.h"

#include <boost/foreach.hpp>

#include <gmock/gmock.h>

#include "consensus/validation.h"
#include "transaction_builder.h"
#include "utiltest.h"
#include "zcash/JoinSplit.hpp"

#include <rust/ed25519.h>

#include "utilstrencodings.h"
#include "serialize.h"
#include "streams.h"

#include <univalue/univalue.h>

#include "myBlock.h"
//#include "myTransaction.h"

using namespace libzcash;
using namespace oabe;
using namespace oabe::crypto;
using namespace std;

void testTransaction()
{
   
}

void testBlock()
{
    
}

int main ()
{
    testTransaction();
    return 0;
}
