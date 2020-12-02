#include "MyBlockChain.h"

MyBlockChain::MyBlockChain(/* args */)
{
    this->lastBlockHash = 0;
    this->vpub = 0;
}

MyBlockChain::~MyBlockChain()
{
    for (size_t i = 0; i < this->blocksArray.size(); i++)
    {
        delete blocksArray[i];
    }
    
    this->blocksArray.clear();
}

MyBlock * MyBlockChain::creatGeniusBlock(std::vector<MyTransaction *> transactionsArray, int amount)
{
    time_t timeStamp = time(nullptr);
    size_t prevHash = this->lastBlockHash;
    MyBlock *geniusBlock = new MyBlock(timeStamp, prevHash, 0, transactionsArray);
    this->appendBlock(geniusBlock);
    this->vpub += amount;
}

MyBlock * MyBlockChain::creatNewBlock(std::vector<MyTransaction *> transactionsArray, int amount)
{
    time_t timeStamp = time(nullptr);
    size_t prevHash = this->lastBlockHash;
    MyBlock *block = new MyBlock(timeStamp, prevHash, 0, transactionsArray);
    this->appendBlock(block);
    this->vpub -= amount;
}

void MyBlockChain::appendBlock(MyBlock *block)
{
    this->blocksArray.push_back(block);
    this->lastBlockHash = block->getBlockHash();
    this->height += 1;
}

uint64_t MyBlockChain::getVpub()
{
    return this->vpub;
}

int MyBlockChain::getHeight()
{
    return this->blocksArray.size();
}

MyBlock * MyBlockChain::queryBlockAtHeight(int height)
{
    if (height < this->getHeight())
    {
        return this->blocksArray[height];
    }
    
    return nullptr;
}

bool MyBlockChain::isNoteNullified(SproutNote note, SproutSpendingKey sk)
{
    uint256 noteNullifier = note.nullifier(sk);
    for (auto &&nullifier : this->nullifiersArray)
    {
        if (nullifier == noteNullifier)
        {
            return true;
        }
        
    }
    return false;
}

void MyBlockChain::appendNullifierArray(std::vector<uint256> nullifierArray)
{
    for (auto &&nullifier : nullifierArray)
    {
        this->nullifiersArray.push_back(nullifier);
    }
}