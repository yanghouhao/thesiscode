#ifndef _AUDITHANDLE_H_
#define _AUDITHANDLE_H_

#include <BaseHandler.h>

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
				if (AuditHandler::instance->model)
				{
					delete AuditHandler::instance->model;
				}
				
				delete AuditHandler::instance;
			}
			
		}
	};
	static CGarbo Garbo;
public:
    static AuditHandler * shareInstance();
	//override
	void handle();
	void inputInfo();
};

#endif