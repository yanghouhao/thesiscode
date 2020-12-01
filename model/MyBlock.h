#include <ctime>
#include <string>
#include <vector>

#include "MyTransaction.h"

class MyBlock
{
private:
    /* data */
    time_t timeStamp;
    size_t hash;
    std::string prevBlockHash;
    int height;
    std::vector<MyTransaction *> transactionsVector;

public:
    //con/destructor
    MyBlock(time_t,
          size_t,
          int,
          std::vector<MyTransaction *>);

    ~MyBlock();

    //property

    //method
    time_t getTimeStamp() const;
    size_t getBlockHash() const;
    std::string getPrevBlockHash() const;
    int getHeight() const;
    std::vector<MyTransaction *> getTransactionsVector() const;
    void caculateHash();
};
