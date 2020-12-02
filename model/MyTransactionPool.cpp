#include "MyTransactionPool.h"
#include "Storage.h"

MyTransactionPool * MyTransactionPool::instance = nullptr;

MyTransactionPool * MyTransactionPool::shareInstance()
{
    if (!instance)
    {
        instance = new MyTransactionPool();
    }
    return instance;
}

void MyTransactionPool::addTransaction(MyTransaction *transaction, int amount)
{
    this->transactionsArray.push_back(transaction);
    this->vpub += amount;

    for (auto &&nullifier : transaction->jsDescription->nullifiers)
    {
        this->nullifiersArray.push_back(nullifier);
    }
    
    if (this->transactionsArray.size() > 0)
    {
        MyBlockChain *blockChain = Storage::shareInstance()->sharedBlockChain();
        blockChain->creatNewBlock(this->transactionsArray, amount);
        blockChain->appendNullifierArray(this->nullifiersArray);
        this->transactionsArray.clear();
        this->nullifiersArray.clear();
        amount = 0;
    }
}
