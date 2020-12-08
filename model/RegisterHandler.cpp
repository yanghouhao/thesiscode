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

void RegisterHandler::handle()
{
    string order = this->model.getOrders().front();

    if (order == "X")
    {
        return;
    }

    string userName;
    cout << "请输入用户名" << endl;
    while (cin >> userName)
    {
        UserModel *user = Storage::shareInstance()->getSingleUserByName(userName);
        if (user)
        {
            cout << "用户名存在" << endl;
        }

        break;
    }

    string userType;
    cout << "请输入用户类型" << endl;
    cout << "client 代表普通用户" << endl;
    cout << "auditor 代表审计员" << endl;
    ClientType type;
    while (cin >> userType)
    {
        if (userType == "client" || userType == "auditor")
        {
            type = userType == "client" ? ClientTypeClient : ClientTypeAuditor;
            break;
        }
        else
        {
            continue;
        }
    }
    
    this->regist(userName, type);
}

void RegisterHandler::inputInfo()
{
    string order;

    while (cin >> order)
    {
        if (!this->isValidInput(order))
        {
            cout << "输入有误，请重新输入" << endl;
            this->printHelp();
            continue;
        }

        if (order == "HELP")
        {
            this->printHelp();
            continue;
        }
        
        this->model = HandlerModel();
        this->model.addOrder(order);
        return;  
    }
    
}

void RegisterHandler::printHelp()
{
    cout << "请输入命令" << endl;
    cout << "regist 代表注册" << endl;
    cout << "HELP 显示帮助" << endl;
    cout << "X 退出" << endl;
}
	
bool RegisterHandler::isValidInput(std::string order)
{
    return order == "regist" || order == "HELP" || order == "X";
}