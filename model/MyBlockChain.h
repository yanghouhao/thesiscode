#include <vector>
#include <string>

#include "MyBlock.h"

class MyBlockChain
{
private:
    /* data */
    std::vector<MyBlock *> blocksArray;
    size_t lastBlockHash;
    int height;
public:
    MyBlockChain(/* args */);
    ~MyBlockChain();
    MyBlock * creatGeniusBlock(int);
    MyBlock * creatNewBlock();
    void appendBlock(MyBlock *);
};
