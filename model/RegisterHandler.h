#ifndef _REGISTERHANDLER_H_
#define _REGISTERHANDLER_H_

#include "Storage.h"
#include "BaseHandler.h"

class RegisterHandler : public BaseHandler
{
private:
    RegisterHandler(){};
    static RegisterHandler *instance;
    class CGarbo   //它的唯一工作就是在析构函数中删除CSingleton的实例
	{
	public:
		~CGarbo()
		{
			if(RegisterHandler::instance)
			{
				delete RegisterHandler::instance;
			}
				
		}
	};
	static CGarbo Garbo;
	void regist(std::string, ClientType);
public:
    static RegisterHandler * shareInstance();
	//override
	void handle();
	void inputInfo();
	void printHelp();
	bool isValidInput(std::string);
};

#endif