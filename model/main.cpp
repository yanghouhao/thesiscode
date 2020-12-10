#include <iostream>
#include <openabe/openabe.h>
#include <openabe/zsymcrypto.h>

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1

#include "librustzcash.h"

#include "TestHandler.h"
#include "AuditHandler.h"
#include "TransactionHandler.h"
#include "RegisterHandler.h"
#include "PrintHandler.h"
#include "BaseHandler.h"

#include "src/key.h"

using namespace oabe;
using namespace oabe::crypto;

using namespace std;

Storage *storage = Storage::shareInstance();
namespace fs = boost::filesystem;

void init()
{
    cout << "初始化中" << endl;
    ECC_Start();
    boost::filesystem::path sapling_spend =  ZC_GetParamsDir() / "sapling-spend.params";
    boost::filesystem::path sapling_output =ZC_GetParamsDir() / "sapling-output.params";
    boost::filesystem::path sprout_groth16 =ZC_GetParamsDir() / "sprout-groth16.params";

    auto sapling_spend_str = sapling_spend.native();
    auto sapling_output_str = sapling_output.native();
    auto sprout_groth16_str = sprout_groth16.native();

    librustzcash_init_zksnark_params(
        reinterpret_cast<const codeunit*>(sapling_spend_str.c_str()),
        sapling_spend_str.length(),
        reinterpret_cast<const codeunit*>(sapling_output_str.c_str()),
        sapling_output_str.length(),
        reinterpret_cast<const codeunit*>(sprout_groth16_str.c_str()),
        sprout_groth16_str.length()
    );
    
    auto blockChain = storage->sharedBlockChain();
    std::vector<MyTransaction *> noTransaction;
    blockChain->creatGeniusBlock(noTransaction, 1000);

    UserModel *client = new UserModel("yangc", ClientTypeClient);
    Storage::shareInstance()->addRegistedUser(client);

    UserModel *client1 = new UserModel("yanghouhao", ClientTypeClient);
    Storage::shareInstance()->addRegistedUser(client1);

    UserModel *auditor = new UserModel("yanga", ClientTypeAuditor);
    auditor->addAuditee(client);
    Storage::shareInstance()->addRegistedUser(auditor);
    InitializeOpenABE();
}

void end()
{
    ECC_Stop();
    ShutdownOpenABE();
}

BaseHandler *handlerFactory(std::string command)
{
    if (command == "regist")
    {
        return RegisterHandler::shareInstance();
    } 
    else if (command == "transaction")
    {
        return TransactionHandler::shareInstance();
    }
    else if (command == "print")
    {
        return PrintHandler::shareInstance();
    }
    else if (command == "audit")
    {
        return AuditHandler::shareInstance();
    }
    else if (command == "test")
    {
        return TestHandler::shareInstance();
    }
    
}

void printHelp()
{
    cout << "请输入命令" << endl;
    cout << "regist 注册用户" << endl;
    cout << "transaction 交易" << endl;
    cout << "print 打印信息" << endl;
    cout << "audit 审计功能" << endl;
    cout << "HELP 帮助" << endl;
    cout << "X 退出" << endl;
    cout << "EXIT 退出" << endl;
}

bool isInputValid(string command)
{
    return command == "test" || command == "HELP" || command == "regist" || command == "transaction" || command == "print" || command == "audit" || command == "X" || command == "EXIT";
}

int main()
{
    init();

    string command;
    cout << "请输入命令，输入 HELP 获取帮助" << endl;
    printHelp();
    while (cin >> command)
    {
        if (!isInputValid(command))
        {
            cout << "输入无效，请重新输入" << endl;
            printHelp();
            continue;
        }

        if (command == "X" || command == "EXIT")
        {
            break;
        }

        if (command == "HELP")
        {
            printHelp();
            continue;
        }
        
        BaseHandler *handler;
        handler = handlerFactory(command);
        if (handler)
        {
            handler->inputInfo();
            handler->handle();
        }
        else
        {
            printHelp();
            continue;
        }
    }
    
    end();

    return 0;
}