#ifndef _TESTHANDLER_H_
#define _TESTHANDLER_H_

#include "BaseHandler.h"

class TestHandler : public BaseHandler
{
private:
    TestHandler(){};
    static TestHandler *instance;
    class CGarbo   //它的唯一工作就是在析构函数中删除CSingleton的实例
	{
	public:
		~CGarbo()
		{
			if(TestHandler::instance)
			{	
				delete TestHandler::instance;
			}
			
		}
	};
	static CGarbo Garbo;
    void testUtils();
public:
    static TestHandler * shareInstance();
	//override
	void handle();
	void inputInfo();
    void printHelp() ;
    bool isValidInput(std::string);
};

#endif