#include "MyBlockChain.h"

#include <vector>

class ServerStorage
{
private:
    /* data */
    ServerStorage(/* args */);
    MyBlockChain *blockChain;
    std::vector<MyTransaction *> transactionPool;
    void createBlock();
public:
    ~ServerStorage();
    ServerStorage * shareInstance();
    void addTransaction(MyTransaction *);
};
