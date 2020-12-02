#ifndef _QUERYHANDLER_H_
#define _QUERYHANDLER_H_

#include <vector>
#include <string>
#include "UserModel.h"
#include "BaseHandler.h"

class QueryHandler : public BaseHandler
{
private:
    QueryHandler(){};
    static QueryHandler *instance;
    class CGarbo   //它的唯一工作就是在析构函数中删除CSingleton的实例
	{
	public:
		~CGarbo()
		{
			if(QueryHandler::instance)
			{
				if (QueryHandler::instance->model)
				{
					delete QueryHandler::instance->model;
				}
				
				delete QueryHandler::instance;
			}
			
		}
	};
	static CGarbo Garbo;
	
public:
    static QueryHandler * shareInstance();
    SproutPaymentAddress * queryUserAddress(std::string);
    SproutSpendingKey * queryUserSpendingKey(std::string);
    int queryUserAccount(std::string);
    bool isAccountMoneyValid(std::string, int);
    std::vector<libzcash::SproutNote> queryUserValidNotesArray(std::string);
	std::vector<libzcash::SproutNote> queryUserAllNotesArray(std::string);

	bool isValidAuditor(std::string);

	//override
	void handle();
	void inputInfo();
	void printHelp();
	bool isValidInput(std::string);
};


#endif

/*
class QueryHandler : public BaseHandler
{
private:
    QueryHandler(){};
    static QueryHandler *instance;
    class CGarbo   //它的唯一工作就是在析构函数中删除CSingleton的实例
	{
	public:
		~CGarbo()
		{
			if(QueryHandler::instance)
			{
				if (QueryHandler::instance->model)
				{
					delete QueryHandler::instance->model;
				}
				
				delete QueryHandler::instance;
			}
			
		}
	};
	static CGarbo Garbo;
public:
    static QueryHandler * shareInstance();
	//override
	void handle();
	void inputInfo();
};

QueryHandler * QueryHandler::instance = nullptr;

QueryHandler * QueryHandler::shareInstance()
{
    if (!instance)
    {
        instance = new QueryHandler();
    }
    return instance;
}
*/ 