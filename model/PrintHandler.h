#ifndef _PRINTHANDLER_H_
#define _PRINTHANDLER_H_

#include "Storage.h"

class PrintHandler
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
				delete PrintHandler::instance;
		}
	};
	static CGarbo Garbo;

    void print(UserModel *);
public:
    static PrintHandler * shareInstance();
    void printSigleUser(std::string);
    void printAllUser();
};

#endif