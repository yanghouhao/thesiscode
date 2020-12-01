#include "MyBlockChain.h"

MyBlockChain::MyBlockChain(/* args */)
{
    this->lastBlockHash = 0;
}

MyBlockChain::~MyBlockChain()
{
    for (size_t i = 0; i < this->blocksArray.size(); i++)
    {
        delete blocksArray[i];
    }
    
    this->blocksArray.clear();
}

MyBlock * MyBlockChain::creatGeniusBlock(int amount)
{
    if (!this->height)
    {
        return nullptr;
    }
    time_t timeStamp = time(nullptr);
    size_t prevHash = this->lastBlockHash;
    std::vector<MyTransaction *> noneTransaction;
    MyBlock *geniusBlock = new MyBlock(timeStamp, prevHash, 0, noneTransaction);
    this->appendBlock(geniusBlock);
}

MyBlock * MyBlockChain::creatNewBlock()
{
    
}

void MyBlockChain::appendBlock(MyBlock *block)
{
    this->blocksArray.push_back(block);
    this->lastBlockHash = block->getBlockHash();
}