#include "Storage.h"
#include "TransactionHandler.h"
#include "MyTransactionPool.h"
#include "Utils.h"

TransactionHandler * TransactionHandler::instance = nullptr;

TransactionHandler * TransactionHandler::shareInstance()
{
    if (!instance)
    {
        instance = new TransactionHandler();
    }
    return instance;
}

void TransactionHandler::create(string recipientName, int amount)
{
    uint64_t vpub_old = Storage::shareInstance()->getVpub();
    uint64_t vpub_new = vpub_old - amount;

    UserModel *recipient = Storage::shareInstance()->getSingleUserByName(recipientName);
    if (!recipient)
    {
        return;
    }
    
    std::array<JSInput, 2> inputs = 
    {
        JSInput(), // dummy input
        JSInput() // dummy input
    };

    std::array<JSOutput, 2> outputs = 
    {
        JSOutput(recipient->getPaymentAddress(), amount),
        JSOutput()// dummy output
    };

    auto joinSplitPubKey = Storage::shareInstance()->getJSPubKey();

    SproutMerkleTree tree;
    uint256 rt = tree.root();

    JSDescription *jsdesc = MyTransaction::makeSproutProof(
            inputs,
            outputs,
            joinSplitPubKey,
            vpub_old,
            vpub_new,
            rt
    );

    uint252 phi = random_uint252();
    uint256 h_sig = jsdesc->h_sig(joinSplitPubKey);
    
    std::vector<SproutNotePlaintext> notesTextArray;
    std::array<unsigned char, ZC_MEMO_SIZE> memo;
    std::vector<string> cipherTextArray;

    string encryptKey = recipientName;

    for (size_t i = 0; i < 2; i++) 
    {
        uint256 r = random_uint256();
        auto note = outputs[i].note(phi, r, i, h_sig);
        SproutNotePlaintext note_pt(note, memo);
        string plainText = Utils::shareInstance()->serializeNote(note_pt);
        string cipherText = Utils::shareInstance()->encrypt(plainText, encryptKey);
        cipherTextArray.push_back(cipherText);
    }

    MyTransaction *transaction = new MyTransaction(jsdesc, cipherTextArray, TransactionTypeTreate);
    
    this->storeTransaction(transaction, amount);
}

void TransactionHandler::transfer(string publisher, string recipient, int amount, std::vector<SproutNote> noteArray)
{
    
}

void TransactionHandler::storeTransaction(MyTransaction *transaction, int amount)
{
    MyTransactionPool *transactionPool = MyTransactionPool::shareInstance();
    transactionPool->addTransaction(transaction, amount);
}