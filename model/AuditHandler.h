#ifndef _AUDITHANDLE_H_
#define _AUDITHANDLE_H_

#include "BaseHandler.h"

class AuditHandler : public BaseHandler
{
private:
    AuditHandler(){};
    static AuditHandler *instance;
    class CGarbo   //它的唯一工作就是在析构函数中删除CSingleton的实例
	{
	public:
		~CGarbo()
		{
			if(AuditHandler::instance)
			{
				delete AuditHandler::instance;
			}
			
		}
	};
	static CGarbo Garbo;

	void showAuditee();
	void showAllNote(std::string);

public:
    static AuditHandler * shareInstance();	
	//override
	void handle();
	void inputInfo();
	void printHelp();
	bool isValidInput(std::string);
};

#endif