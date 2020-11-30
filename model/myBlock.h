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
    //con/destructor
    myBlock(time_t,
          std::string,
          int,
          std::vector<myTransaction *>);

    ~myBlock();

    //property

    //method
    time_t getTimeStamp() const;
    size_t getBlockHash() const;
    std::string getPrevBlockHash() const;
    int getHeight() const;
    std::vector<myTransaction *> getTransactionsVector() const;
    void caculateHash();
};
