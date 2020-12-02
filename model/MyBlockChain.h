#ifndef _MYBLOCKCHAIN_H_
#define _MYBLOCKCHAIN_H_

#include <vector>
#include <string>

#include "MyBlock.h"

class MyBlockChain
{
private:
    /* data */
    std::vector<MyBlock *> blocksArray;
    std::vector<uint256> nullifiersArray;
    size_t lastBlockHash;
    int height;
    uint64_t vpub;
public:
    MyBlockChain(/* args */);
    ~MyBlockChain();
    MyBlock * creatGeniusBlock(std::vector<MyTransaction *>, int);
    MyBlock * creatNewBlock(std::vector<MyTransaction *>, int);
    void appendBlock(MyBlock *);
    uint64_t getVpub();
    int getHeight();
    bool isNoteNullified(SproutNote, SproutSpendingKey);
    void appendNullifierArray(std::vector<uint256>);
    MyBlock * queryBlockAtHeight(int);
};

#endif
