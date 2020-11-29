#include <ctime>
#include <string>
#include <vector>

#include "myTransaction.h"

class myBlock
{
private:
    /* data */
    time_t timeStamp;
    size_t hash;
    std::string prevBlockHash;
    int height;
    std::vector<myTransaction *> transactionsVector;

public:
    myBlock(time_t,
          std::string,
          int,
          std::vector<myTransaction *>);

    ~myBlock();
    time_t getTimeStamp() const;
    size_t getBlockHash() const;
    std::string getPrevBlockHash() const;
    int getHeight() const;
    std::vector<myTransaction *> getTransactionsVector() const;
    void caculateHash();
};

myBlock::myBlock(time_t timeStamp,
             std::string prevBlockHash,
             int height,
             std::vector<myTransaction *> transactionsVector)
{
    this->timeStamp = timeStamp;
    this->prevBlockHash = prevBlockHash;
    this->height = height;
    this->transactionsVector = transactionsVector;
    this->caculateHash();
}

myBlock::~myBlock()
{
}

time_t myBlock::getTimeStamp() const
{
    return this->timeStamp;
}

size_t myBlock::getBlockHash() const
{
    return this->hash;
}

std::string myBlock::getPrevBlockHash() const
{
    return this->prevBlockHash;
}

int myBlock::getHeight() const
{
    return this->height;
}

std::vector<myTransaction *> myBlock::getTransactionsVector() const
{
    return this->transactionsVector;
}

void myBlock::caculateHash()
{
    size_t timeStamp = std::hash<time_t>()(this->timeStamp);
    size_t prevBlockHash = std::hash<std::string>()(this->prevBlockHash);
    size_t height = std::hash<int>()(this->height);
    this->hash = timeStamp ^ prevBlockHash ^ height;
}
