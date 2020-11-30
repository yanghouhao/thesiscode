#include "myBlockChain.h"

myBlockChain::myBlockChain(/* args */)
{
    this->lastBlockHash = 0;
}

myBlockChain::~myBlockChain()
{
    for (size_t i = 0; i < this->blocksArray.size(); i++)
    {
        delete blocksArray[i];
    }
    
    this->blocksArray.clear();
}

myBlock * myBlockChain::creatGeniusBlock(int amount)
{
    if (!this->height)
    {
        return nullptr;
    }
    time_t timeStamp = time(nullptr);
    size_t prevHash = this->lastBlockHash;
    std::vector<myTransaction *> noneTransaction;
    myBlock *geniusBlock = new myBlock(timeStamp, prevHash, 0, noneTransaction);
    this->appendBlock(geniusBlock);
}

myBlock * myBlockChain::creatNewBlock()
{
    
}

void myBlockChain::appendBlock(myBlock *block)
{
    this->blocksArray.push_back(block);
    this->lastBlockHash = block->getBlockHash();
}