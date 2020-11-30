#include <vector>
#include <string>

#include "myBlock.h"

class myBlockChain
{
private:
    /* data */
    std::vector<myBlock *> blocksArray;
    size_t lastBlockHash;
    int height;
public:
    myBlockChain(/* args */);
    ~myBlockChain();
    myBlock * creatGeniusBlock(int);
    myBlock * creatNewBlock();
    void appendBlock(myBlock *);
};
