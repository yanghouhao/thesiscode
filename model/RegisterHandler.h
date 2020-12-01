#ifndef _REGISTERHANDLER_H_
#define _REGISTERHANDLER_H_

#include "Storage.h"

class RegisterHandler
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
				delete RegisterHandler::instance;
		}
	};
	static CGarbo Garbo;
public:
    static RegisterHandler * shareInstance();
    static void regist(std::string, ClientType);
};

#endif
