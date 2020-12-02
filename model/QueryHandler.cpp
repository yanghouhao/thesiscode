#include "QueryHandler.h"
#include "Utils.h"
#include "Storage.h"

QueryHandler * QueryHandler::instance = nullptr;

QueryHandler * QueryHandler::shareInstance()
{
    if (!instance)
    {
        instance = new QueryHandler();
    }
    return instance;
}

SproutPaymentAddress * QueryHandler::queryUserAddress(std::string userName)
{
    UserModel *user = Storage::shareInstance()->getSingleUserByName(userName);
    return new SproutPaymentAddress(user->getPaymentAddress());
}

SproutSpendingKey * QueryHandler::queryUserSpendingKey(std::string userName)
{
    UserModel *user = Storage::shareInstance()->getSingleUserByName(userName);
    return new SproutSpendingKey(user->getSpendingKey());
}

int QueryHandler::queryUserAccount(std::string userName)
{
    std::vector<libzcash::SproutNote> noteArray = this->queryUserValidNotesArray(userName);
    int res = 0;

    for (auto &&note : noteArray)
    {
        res += note.value();
    }
    
    return res;
}

bool QueryHandler::isAccountMoneyValid(std::string userName, int amount)
{
    return this->queryUserAccount(userName) >= amount;
}

std::vector<libzcash::SproutNote> QueryHandler::queryUserValidNotesArray(std::string userName)
{
    std::vector<libzcash::SproutNote> res;

    SproutPaymentAddress *paymentAddress = this->queryUserAddress(userName);
    SproutSpendingKey *spendingKey = this->queryUserSpendingKey(userName);

    MyBlockChain *bc = Storage::shareInstance()->sharedBlockChain();
    int blockHeight = bc->getHeight();

    for (size_t i = 0; i < blockHeight; i++)
    {
        MyBlock *block = bc->queryBlockAtHeight(i);
        std::vector<MyTransaction *> transactionsArray = block->getTransactionsVector();
        
        for (auto &&transaction : transactionsArray)
        {
            vector<string> cipherTextArray = transaction->cipherTextArrayWithABE;

            for (auto &&ciphertext : cipherTextArray)
            {
                Utils *util = Utils::shareInstance();
                string note_string = util->decrypt(userName, ciphertext);
                SproutNotePlaintext note_pt = util->deserializeNote(note_string);
                SproutNote note = note_pt.note(*paymentAddress);
                if (!bc->isNoteNullified(note, *spendingKey) && note.a_pk == paymentAddress->a_pk)
                {
                    res.push_back(note);
                }
            }
        }
    }
    
    delete paymentAddress;
    delete spendingKey;
    return res;
}

void QueryHandler::handle()
{

}

void QueryHandler::inputInfo()
{

}