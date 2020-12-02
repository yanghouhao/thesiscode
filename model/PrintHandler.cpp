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
    if (!userArray.size())
    {
        cout << "没有注册的用户" << endl;
    }
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
            return;
        }
    }
    cout << "没有这个用户" << endl;
}

void PrintHandler::print(UserModel *user)
{
    cout << "username : " << user->getName() << endl;
    string userType = user->getUserType() == client ? "client" : "auditor";
    cout << "userType : " << userType << endl;
    cout << "-----------------------------" << endl;
}

void PrintHandler::printBlockChainInfo()
{
    MyBlockChain *bc = Storage::shareInstance()->sharedBlockChain();
    cout << "blockchain height is : " << bc->getHeight() << endl;
    cout << "transparent pool have : " << bc->getVpub() << endl;
    cout << "-----------------------------" << endl;
}

void PrintHandler::handle()
{
    if (!this->model)
    {
        cout << "输入有误，请重新输入" << endl;
        return;
    }

    for (auto &&order : this->model->getOrders())
    {
        if (order == "BLOCKCHAIN")
        {
            this->printBlockChainInfo();
            return;
        }
        
        if (order == "ALL")
        {
            this->printAllUser();
            return;
        }
        else
        {
            this->printSigleUser(order);
        }
    }
}

void PrintHandler::inputInfo()
{
    string printObject;
    cout << "请输入要打印的对象" << endl;
    cout << "user 代表需要打印用户信息" << endl;
    cout << "blockchain 代表区块链信息" << endl;

    cin >> printObject;
    if (printObject == "user")
    {
        this->printUser();
    }
    else if (printObject == "blockchain")
    {
        this->printBlockchain();
    }
}

void PrintHandler::printUser()
{
    string userName;
    cout << "请输入用户名指定查询某一用户，或者输入 ALL 查询所有用户" << endl;
    cin >> userName;

    if (this->model)
    {
        delete this->model;
    }
    
    vector<string> orders;
    orders.push_back(userName);
    this->model = new HandlerModel(orders);
}

void PrintHandler::printBlockchain()
{
    if (this->model)
    {
        delete this->model;
    }
    
    vector<string> orders;
    orders.push_back("BLOCKCHAIN");
    this->model = new HandlerModel(orders);
}