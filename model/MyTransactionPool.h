#ifndef _MYTRANSACTIONPOOL_H_
#define _MYTRANSACTIONPOOL_H_

#include <vector>

#include "MyTransaction.h"

class MyTransactionPool
{
private:
    MyTransactionPool(){
        this->vpub = 0;
    };
    static MyTransactionPool *instance;
    class CGarbo   //它的唯一工作就是在析构函数中删除CSingleton的实例
	{
	public:
		~CGarbo()
		{
			if(MyTransactionPool::instance)
            {
                for (auto &&transaction : MyTransactionPool::instance->transactionsArray)
                {
                    delete transaction;
                }
                
                delete MyTransactionPool::instance;
            }
		}
	};
	static CGarbo Garbo;

    int vpub;

    vector<MyTransaction *> transactionsArray;

public:
    static MyTransactionPool * shareInstance();
    void addTransaction(MyTransaction *, int);
};


#endif