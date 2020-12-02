#ifndef _TRANSACTIONHANDLER_H_
#define _TRANSACTIONHANDLER_H_

#include "MyTransaction.h"
#include "BaseHandler.h"

class TransactionHandler : public BaseHandler
{
private:
    TransactionHandler(){};
    static TransactionHandler *instance;
    class CGarbo   //它的唯一工作就是在析构函数中删除CSingleton的实例
	{
	public:
		~CGarbo()
		{
			if(TransactionHandler::instance)
			{
				if (TransactionHandler::instance->model)
				{
					delete TransactionHandler::instance->model;
				}
				delete TransactionHandler::instance;
			}
		}
	};
	static CGarbo Garbo;

    void storeTransaction(MyTransaction *, int);
    MyTransaction * createTransaction();

public:
    static TransactionHandler * shareInstance();
    void transfer(string, string, int, std::vector<SproutNote>);
    void create(string, int);
	//override
	void handle();
	void inputInfo();
	void printHelp();
	bool isValidInput(std::string);
};

#endif