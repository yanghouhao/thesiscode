#include "RegisterHandler.h"

RegisterHandler * RegisterHandler::instance = nullptr;

RegisterHandler * RegisterHandler::shareInstance()
{
    if (!instance)
    {
        instance = new RegisterHandler();
    }
    return instance;
}

void RegisterHandler::regist(std::string userName, ClientType clientType)
{
    UserModel *user = new UserModel(userName, clientType);
    Storage::shareInstance()->addRegistedUser(user);
}