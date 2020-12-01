#include "PrintHandler.h"
#include "Storage.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

PrintHandler * PrintHandler::instance = nullptr;

PrintHandler * PrintHandler::shareInstance()
{
    if (!instance)
    {
        instance = new PrintHandler();
    }
    return instance;
}

void PrintHandler::printAllUser()
{
    auto userArray = Storage::shareInstance()->getRegistedUserArray();
    for (auto &&useItem : userArray)
    {
        this->print(useItem);
    }
}

void PrintHandler::printSigleUser(std::string userName)
{
    auto userArray = Storage::shareInstance()->getRegistedUserArray();
    for (auto &&useItem : userArray)
    {
        if (userName == useItem->getName())
        {
            this->print(useItem);
        }
    }
}

void PrintHandler::print(UserModel *user)
{
    cout << "username : " << user->getName() << endl;
    string userType = user->getUserType() == client ? "client" : "auditor";
    cout << "userType : " << userType << endl;
    
}