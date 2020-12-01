#include "MyTransactionPool.h"
#include "Storage.h"

MyTransactionPool * MyTransactionPool::instance = nullptr;

void MyTransactionPool::addTransaction(MyTransaction *transaction, int amount)
{
    this->transactionsArray.push_back(transaction);
    this->vpub += amount;
    if (this->transactionsArray.size() > 3)
    {
        MyBlockChain *blockChain = Storage::shareInstance()->sharedBlockChain();
        blockChain->creatNewBlock(this->transactionsArray, amount);
        this->transactionsArray.clear();
        amount = 0;
    }
    
}