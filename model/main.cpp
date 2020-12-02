#include <iostream>

#include "librustzcash.h"

#include "TransactionHandler.h"
#include "RegisterHandler.h"
#include "PrintHandler.h"
#include "BaseHandler.h"

#include "src/key.h"

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
}

void end()
{
    ECC_Stop();
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
}

int main()
{
    init();

    string command;
    BaseHandler *handler;
    
    while (cout << "请输入命令，输入 HELP 获取帮助" << endl , cin >> command)
    {
        if (command == "X" or command == "EXIT")
        {
            break;
        }

        handler = handlerFactory(command);
        handler->inputInfo();
        handler->handle();
    }
    
    end();

    return 0;
}