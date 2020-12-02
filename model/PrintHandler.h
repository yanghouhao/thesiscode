#ifndef _PRINTHANDLER_H_
#define _PRINTHANDLER_H_

#include "Storage.h"
#include "BaseHandler.h"

class PrintHandler : public BaseHandler
{
private:
    PrintHandler(){};
    static PrintHandler *instance;
    class CGarbo   //它的唯一工作就是在析构函数中删除CSingleton的实例
	{
	public:
		~CGarbo()
		{
			if(PrintHandler::instance)
			{
				if (PrintHandler::instance->model)
				{
					delete PrintHandler::instance->model;
				}
				
				delete PrintHandler::instance;
			}
			
		}
	};
	static CGarbo Garbo;
    void print(UserModel *);
	void printUser();
	void printSigleUser(std::string);
    void printAllUser();

	void printBlockchain();
	void printBlockChainInfo();
public:
    static PrintHandler * shareInstance();
	//override
	void handle();
	void inputInfo();
};

#endif