#include "TestHandler.h"
#include "Storage.h"
#include "UserModel.h"
#include "Utils.h"
#include "TransactionHandler.h"

using namespace std;

TestHandler * TestHandler::instance = nullptr;

TestHandler * TestHandler::shareInstance()
{
    if (!instance)
    {
        instance = new TestHandler();
    }
    return instance;
}

void TestHandler::testUtils()
{
    cout << "-------------------------------" << endl;
    UserModel *client = new UserModel("yangc", ClientTypeClient);
    Storage::shareInstance()->addRegistedUser(client);
    cout << "User generate success" << endl;
    cout << "-------------------------------" << endl;

    UserModel *auditor = new UserModel("yanga", ClientTypeAuditor);
    auditor->addAuditee(client);
    cout << "add auditee success" << endl;
    Storage::shareInstance()->addRegistedUser(auditor);

    cout << "auditor generate success" << endl;
    cout << "-------------------------------" << endl;

    string pt = "im plaintext";
    cout << "encrypting" << endl;
    string ct = Utils::shareInstance()->encrypt(client->getName(),pt);
    cout << "ciphertext is :" << ct << endl;

    string pt1 = Utils::shareInstance()->decrypt(auditor->getAttribute(), ct);
    cout << pt1 << endl;
    cout << "end" << endl;
}

void testCreat()
{
    TransactionHandler::shareInstance()->create("yangc", 100);
    MyBlock *block = Storage::shareInstance()->sharedBlockChain()->queryBlockAtHeight(1);
    MyTransaction *transaction = block->getTransactionsVector()[0];
    auto jsdesc = *transaction->jsDescription;

    for (auto &&cipherText : transaction->cipherTextArrayWithABE)
    {
        auto plaintText = Utils::shareInstance()->decrypt("yangc", cipherText);
        
        auto note = Utils::shareInstance()->deserializeNote(plaintText);
        //cout << "value : " << hex <<  note.value() << endl;
        //cout << "rho : " << note.rho.ToString() << endl;
        //cout << "r : " << note.r.ToString() << endl;
    }
    
}

void TestHandler::handle()
{
    //this->testUtils();
    testCreat();
}

void TestHandler::inputInfo()
{

}

void TestHandler::printHelp() 
{

}

bool TestHandler::isValidInput(std::string)
{
    return true;
}
