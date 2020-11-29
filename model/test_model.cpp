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

//#include "myBlock.h"
//#include "myTransaction.h"

using namespace libzcash;
using namespace oabe;
using namespace oabe::crypto;
using namespace std;

void testTransaction()
{
    InitializeOpenABE();

  cout << "Testing CP-ABE context" << endl;

  OpenABECryptoContext cpabe("CP-ABE");

  string ct, pt1 = "hello world!", pt2;

  cpabe.generateParams();

  cpabe.keygen("|attr1|attr2", "key0");

  cpabe.encrypt("attr1 and attr2", pt1, ct);

  bool result = cpabe.decrypt("key0", ct, pt2);

 cout << pt2 << endl;

  //assert(result && pt1 == pt2);

  cout << "Recovered message: " << pt2 << endl;

  ShutdownOpenABE();
    {
    // Create verification context.
    auto verifier = ProofVerifier::Strict();

    // The recipient's information.
    SproutSpendingKey recipient_key = SproutSpendingKey::random();
    SproutPaymentAddress recipient_addr = recipient_key.address();
    // Create the commitment tree
    SproutMerkleTree tree;

    // Set up a JoinSplit description
    uint64_t vpub_old = 100;
    uint64_t vpub_new ;
   // Ed25519SigningKey joinSplitPrivKey;
    Ed25519VerificationKey joinSplitPubKey;
    //ed25519_generate_keypair(&joinSplitPrivKey, &joinSplitPubKey);
    GetRandBytes(joinSplitPubKey.bytes, ED25519_VERIFICATION_KEY_LEN);
    uint256 rt = tree.root();
    JSDescription jsdesc;
    int a,b;
    {
        cout<<"透明池向隐藏地址A转账"<<endl;
        cout<<"请输入A账号初始化金额数值"<<endl;
        cin>>a;
        cout<<"A账号初始化金额数值为"<<a<<endl;
        vpub_new=vpub_old-a;
        std::array<JSInput, 2> inputs = {
            JSInput(), // dummy input
            JSInput() // dummy input
        };

        std::array<JSOutput, 2> outputs = {
                JSOutput(recipient_addr, a),
                JSOutput()// dummy output
        };

        std::array<SproutNote, 2> output_notes;

        // Perform the proofs
        jsdesc = myTransaction::makeSproutProof(
            inputs,
            outputs,
            joinSplitPubKey,
            vpub_old,
            vpub_new,
            rt
        );

        cout<<"零知识证明由以下内容生成"<<endl;
        cout<<"透明池输入"<<vpub_old<<endl;
        cout<<"透明池输出"<<vpub_new<<endl;
        cout<<"隐藏地址input金额:"<<inputs.data()->note.value()<<endl;
        cout<<"隐藏地址output金额:"<<outputs.data()->value<<endl;
        cout<<"默克树根值"<<rt.ToString()<<endl;
        cout<<"\n\t"<<endl;

    }
    cout<<"生成交易结构中内容"<<endl;
    cout<<"生成的交易anchor值："<<jsdesc.anchor.GetHex()<<endl;
    cout<<"生成的交易commit[0]值："<<jsdesc.commitments[0].ToString()<<endl;
    cout<<"生成的交易commit[1]值："<<jsdesc.commitments[1].ToString()<<endl;
//    cout<<"生成的交易output密文[0]内容："<<jsdesc.ciphertexts[0].data()<<endl;
    cout<<"生成的交易交换密钥公钥值："<<jsdesc.ephemeralKey.ToString()<<endl;
    cout<<"生成的交易nullifier值："<<jsdesc.nullifiers.data()->ToString()<<endl;
    cout<<"交易signature:"<<jsdesc.h_sig(joinSplitPubKey).ToString()<<endl;
    cout<<"透明池输入:"<<jsdesc.vpub_old<<endl;
    cout<<"透明池输出："<<jsdesc.vpub_new<<endl;
    // Verify both PHGR and Groth Proof:
    {
        SproutMerkleTree tree;
        JSDescription jsdesc2;
        // Recipient should decrypt
        // Now the recipient should spend the money again
        auto h_sig = ZCJoinSplit::h_sig(jsdesc.randomSeed, jsdesc.nullifiers, joinSplitPubKey);
        ZCNoteDecryption  decryptor(recipient_key.receiving_key());

        auto note_pt = SproutNotePlaintext::decrypt(
            decryptor,
            jsdesc.ciphertexts[0],
            jsdesc.ephemeralKey,
            h_sig,
            0
        );

        auto decrypted_note = note_pt.note(recipient_addr);
        cout<<"通过解密ouput中密文得到commit值："<<decrypted_note.cm().ToString()<<endl;
        cout<<"通过解密ouput中密文得到金额："<< decrypted_note.value()<<endl;

        cout<<"默克树根原内容："<<tree.root().ToString()<<endl;
        cout<<"默克树追加上次交易中commit内容"<<endl;
        // Insert the commitments from the last tx into the tree
        tree.append(jsdesc.commitments[0]);
        auto witness_recipient = tree.witness();
        tree.append(jsdesc.commitments[1]);
        witness_recipient.append(jsdesc.commitments[1]);
        vpub_old = 0;
        vpub_new = 0;
        rt = tree.root();

        cout<<"默克树根内容："<<rt.ToString()<<endl;
        Ed25519VerificationKey joinSplitPubKey2;
        GetRandBytes(joinSplitPubKey2.bytes, ED25519_VERIFICATION_KEY_LEN);

        {
            std::array<JSInput, 2> inputs = {
                JSInput(), // dummy input
                JSInput(witness_recipient, decrypted_note, recipient_key)
            };

            SproutSpendingKey second_recipient = SproutSpendingKey::random();
            SproutPaymentAddress second_addr = second_recipient.address();
            cout<<"请输入A向B转账金额数值"<<endl;
            cin>>b;
            cout<<"A向B转账"<<b<<",剩余"<<a-b<<endl;
            std::array<JSOutput, 2> outputs = {
                JSOutput(second_addr, b),
                JSOutput(recipient_addr,a-b) ,// dummy output

            };

            std::array<SproutNote, 2> output_notes;

            // Perform the proofs
            jsdesc2 = myTransaction::makeSproutProof(
                inputs,
                outputs,
                joinSplitPubKey2,
                vpub_old,
                vpub_new,
                rt
            );
            SproutNote note=JSInput(witness_recipient, decrypted_note, recipient_key).note;
            cout<<"生成的交易input中commit值（与上笔交易output的commit值相对应）："<< note.cm().ToString()<<endl;
            cout<<"生成的交易anchor值（与上笔交易的默克树根相对应）："<<jsdesc2.anchor.GetHex()<<endl;
            cout<<"生成的交易commit[0]值："<<jsdesc2.commitments[0].ToString()<<endl;
            cout<<"生成的交易commit[1]值："<<jsdesc2.commitments[1].ToString()<<endl;
            ZCNoteDecryption decryptor2(second_recipient.receiving_key());
            auto h_sig2 = ZCJoinSplit::h_sig(jsdesc2.randomSeed, jsdesc2.nullifiers, joinSplitPubKey2);
            auto note_pt2 = SproutNotePlaintext::decrypt(
                    decryptor,
                    jsdesc2.ciphertexts[1],
                    jsdesc2.ephemeralKey,
                    h_sig2,
                    1  //TODO 随机数会递增
            );
            auto note_pt3 = SproutNotePlaintext::decrypt(
                    decryptor2,
                    jsdesc2.ciphertexts[0],
                    jsdesc2.ephemeralKey,
                    h_sig2,
                    0  //TODO 随机数会递增
            );
            auto decrypted_note2 = note_pt2.note(recipient_key.address());
            auto decrypted_note3 = note_pt3.note(second_recipient.address());
            cout<<"解密output密文得到A账号余额"<<decrypted_note2.value()<<endl;
            cout<<"解密output密文得到B账号余额"<<decrypted_note3.value()<<endl;
        }

        // Verify Groth Proof:
    }
}
}

void testBlock()
{
    
}

int main ()
{
    testTransaction();
    return 0;
}
