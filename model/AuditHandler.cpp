#include "AuditHandler.h"
#include "PrintHandler.h"
#include "QueryHandler.h"
#include <iostream>

using namespace std;

AuditHandler * AuditHandler::instance = nullptr;

AuditHandler * AuditHandler::shareInstance()
{
    if (!instance)
    {
        instance = new AuditHandler();
    }
    return instance;
}

void AuditHandler::handle()
{
    if (!this->model || this->model->getOrders().size() != 2)
    {
        cout << "输入有误，请重新输入" << endl;
        return;
    }

    string order = this->model->getOrders()[1];

    if (order == "X")
    {
        return;
    }
    

    if (order == "ALL")
    {
        this->showAuditee();
    }
    else
    {
        if (!Storage::shareInstance()->getSingleUserByName(order))
        {
            cout << "没有这个用户，请确认" << endl;
            return;
        }
        this->showAllNote(order);
    }
}

void AuditHandler::inputInfo()
{
    if (this->model)
    {
        delete this->model;
    }
    string s;
    cout << endl << "请输入审计员姓名： ";
    cin >> s;

    if (!QueryHandler::shareInstance()->isValidAuditor(s))
    {
        cout << endl << "审计员姓名无效" << endl;
        return;
    }

    string order;
    cout << "审计员" << Storage::shareInstance()->getSingleUserByName(s)->getName() << " 请输入命令" << endl;
    
    while (cin >> order)
    {
        this->printHelp();
        if (this->isValidInput(order))
        {
            this->model = new HandlerModel();
            this->model->addOrder(order);
            return;
        }
        else if (order == "HELP")
        {
            this->printHelp();
        }
    }
}

void AuditHandler::showAllNote(std::string userName)
{
    PrintHandler::shareInstance()->printAllTransaction(userName);
}

void AuditHandler::showAuditee()
{
    PrintHandler::shareInstance()->printAuditee(this->model->getOrders().front());
}

void AuditHandler::printHelp()
{
    cout << "ALL 代表显示所有可审计的成员姓名" << endl;
    cout << "成员姓名 可显示该成员所有交易的Note信息" << endl;
    cout << "X 返回开始" << endl;
}

bool AuditHandler::isValidInput(std::string order)
{
    return order == "ALL" || order != "X";
}