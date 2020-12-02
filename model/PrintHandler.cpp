#include "PrintHandler.h"
#include "QueryHandler.h"
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
    string userType = user->getUserType() == ClientTypeClient ? "client" : "auditor";
    cout << "userType : " << userType << endl;
    cout << "userAccount : " << QueryHandler::shareInstance()->queryUserAccount(user->getName()) << endl;
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
        if (order == "X")
        {
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
    if (this->model)
    {
        delete this->model;
    }

    string printObject;
    this->printHelp();

    while (cin >> printObject)
    {
        if (this->isValidInput(printObject))
        {
            if (printObject == "user")
            {
                this->printUser();
                return;
            }
            else if (printObject == "blockchain")
            {
                this->printBlockchain();
                return;
            }
            else if (printObject == "HELP")
            {
                this->printHelp();
            } else if (printObject == "X")
            {
                this->model = new HandlerModel();
                this->model->addOrder(printObject);
                return;
            }
            
        }
        else
        {
            cout << "输入有误，请重新输入， 输入 HELP 查看帮助" << endl;
        }
        
    }
}

void PrintHandler::printUser()
{
    string userName;
    cout << "请输入用户名指定查询某一用户，或者输入 ALL 查询所有用户" << endl;
    cin >> userName;
    
    vector<string> orders;
    orders.push_back(userName);
    this->model = new HandlerModel(orders);
}

void PrintHandler::printBlockchain()
{
    vector<string> orders;
    orders.push_back("BLOCKCHAIN");
    this->model = new HandlerModel(orders);
}

void PrintHandler::printAuditee(std::string userName)
{
    UserModel *user = Storage::shareInstance()->getSingleUserByName(userName);
    
    vector<UserModel *> auditeeArray = user->getAuditeeArray();
    cout << "以下是你可审计的用户" << endl;
    for (auto &&auditee : auditeeArray)
    {
        cout << auditee->getName() << "  ";
    }
    cout << endl << "-----------------------------" << endl;
}

void PrintHandler::printAllTransaction(std::string userName)
{
    std::vector<libzcash::SproutNote> noteArray = QueryHandler::shareInstance()->queryUserAllNotesArray(userName);
    int i = 1;
    for (auto &&note : noteArray)
    {
        cout << "以下是第" << i << "个Note的信息" << endl;
        this->printNote(note);
    }
    
}

void PrintHandler::printNote(SproutNote note)
{
    cout << "-----------------------------" << endl;
    cout << "Note 中的公钥为: " << note.a_pk.ToString() << endl;
    cout << "Note 中的承诺为: " << note.cm().ToString() << endl;
    cout << "Note 中的随机数r为: " << note.r.ToString() << endl;
    cout << "Note 中的随机数rho为: " << note.rho.ToString() << endl;
    cout << "Note 中的金额为: " << note.value() << endl;
    cout << "-----------------------------" << endl;
}

void PrintHandler::printHelp()
{
    cout << "请输入要打印的对象" << endl;
    cout << "user 代表需要打印用户信息" << endl;
    cout << "blockchain 代表区块链信息" << endl;
    cout << "HELP 显示帮助" << endl;
    cout << "X 退出" << endl;
}
	
bool PrintHandler::isValidInput(std::string order)
{
    return order == "blockchain" || order == "HELP" || order == "user" || order == "X";
}