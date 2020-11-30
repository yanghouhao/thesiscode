#include <vector>
#include <string>

#include "myBlock.h"

class myBlockChain
{
private:
    /* data */
    std::vector<myBlock *> blocks;
    size_t lastBlockHash;
    int height;
public:
    myBlockChain(/* args */);
    ~myBlockChain();
    myBlock * creatGeniusBlock(int);
    void appendBlock(myBlock *);
};
