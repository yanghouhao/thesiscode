#include <AuditHandler.h>

AuditHandler * AuditHandler::instance = nullptr;

AuditHandler * AuditHandler::shareInstance()
{
    if (!instance)
    {
        instance = new AuditHandler();
    }
    return instance;
}