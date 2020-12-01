#include "MyBlock.h"

MyBlock::MyBlock(time_t timeStamp,
             size_t prevBlockHash,
             int height,
             std::vector<MyTransaction *> transactionsVector)
{
    this->timeStamp = timeStamp;
    this->prevBlockHash = prevBlockHash;
    this->height = height;
    this->transactionsVector = transactionsVector;
    this->caculateHash();
}

MyBlock::~MyBlock()
{
    for (auto &&transaction : this->transactionsVector)
    {
        delete transaction;
    }
    
}

time_t MyBlock::getTimeStamp() const
{
    return this->timeStamp;
}

size_t MyBlock::getBlockHash() const
{
    return this->hash;
}

std::string MyBlock::getPrevBlockHash() const
{
    return this->prevBlockHash;
}

int MyBlock::getHeight() const
{
    return this->height;
}

std::vector<MyTransaction *> MyBlock::getTransactionsVector() const
{
    return this->transactionsVector;
}

void MyBlock::caculateHash()
{
    size_t timeStamp = std::hash<time_t>()(this->timeStamp);
    size_t prevBlockHash = std::hash<std::string>()(this->prevBlockHash);
    size_t height = std::hash<int>()(this->height);
    this->hash = timeStamp ^ prevBlockHash ^ height;
}