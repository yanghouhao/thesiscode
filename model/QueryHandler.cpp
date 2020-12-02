#include "QueryHandler.h"
#include "Utils.h"
#include "Storage.h"

QueryHandler * QueryHandler::instance = nullptr;

QueryHandler * QueryHandler::shareInstance()
{
    if (!instance)
    {
        instance = new QueryHandler();
    }
    return instance;
}

SproutPaymentAddress * QueryHandler::queryUserAddress(std::string userName)
{
    UserModel *user = Storage::shareInstance()->getSingleUserByName(userName);
    return new SproutPaymentAddress(user->getPaymentAddress());
}

SproutSpendingKey * QueryHandler::queryUserSpendingKey(std::string userName)
{
    UserModel *user = Storage::shareInstance()->getSingleUserByName(userName);
    return new SproutSpendingKey(user->getSpendingKey());
}

int QueryHandler::queryUserAccount(std::string userName)
{
    std::vector<libzcash::SproutNote> noteArray = this->queryUserValidNotesArray(userName);
    int res = 0;

    for (auto &&note : noteArray)
    {
        res += note.value();
    }
    
    return res;
}

bool QueryHandler::isAccountMoneyValid(std::string userName, int amount)
{
    return this->queryUserAccount(userName) >= amount;
}

std::vector<libzcash::SproutNote> QueryHandler::queryUserValidNotesArray(std::string userName)
{
    std::vector<libzcash::SproutNote> res;

    SproutPaymentAddress *paymentAddress = this->queryUserAddress(userName);
    SproutSpendingKey *spendingKey = this->queryUserSpendingKey(userName);

    MyBlockChain *bc = Storage::shareInstance()->sharedBlockChain();
    int blockHeight = bc->getHeight();

    for (size_t i = 0; i < blockHeight; i++)
    {
        MyBlock *block = bc->queryBlockAtHeight(i);
        std::vector<MyTransaction *> transactionsArray = block->getTransactionsVector();
        
        for (auto &&transaction : transactionsArray)
        {
            vector<string> cipherTextArray = transaction->cipherTextArrayWithABE;

            for (auto &&ciphertext : cipherTextArray)
            {
                Utils *util = Utils::shareInstance();
                string note_string = util->decrypt(userName, ciphertext);
                if (note_string == "decrypt fail")
                {
                    continue;
                }
                
                SproutNotePlaintext note_pt = util->deserializeNote(note_string);
                SproutNote note = note_pt.note(*paymentAddress);
                if (!bc->isNoteNullified(note, *spendingKey) && note.a_pk == paymentAddress->a_pk)
                {
                    res.push_back(note);
                }
            }
        }
    }
    
    delete paymentAddress;
    delete spendingKey;
    return res;
}

std::vector<libzcash::SproutNote> QueryHandler::queryUserAllNotesArray(std::string userName)
{
    std::vector<libzcash::SproutNote> res;

    SproutPaymentAddress *paymentAddress = this->queryUserAddress(userName);
    SproutSpendingKey *spendingKey = this->queryUserSpendingKey(userName);

    MyBlockChain *bc = Storage::shareInstance()->sharedBlockChain();
    int blockHeight = bc->getHeight();

    for (size_t i = 0; i < blockHeight; i++)
    {
        MyBlock *block = bc->queryBlockAtHeight(i);
        std::vector<MyTransaction *> transactionsArray = block->getTransactionsVector();
        
        for (auto &&transaction : transactionsArray)
        {
            vector<string> cipherTextArray = transaction->cipherTextArrayWithABE;

            for (auto &&ciphertext : cipherTextArray)
            {
                Utils *util = Utils::shareInstance();
                string note_string = util->decrypt(userName, ciphertext);
                if (note_string == "decrypt fail")
                {
                    continue;
                }
                
                SproutNotePlaintext note_pt = util->deserializeNote(note_string);
                SproutNote note = note_pt.note(*paymentAddress);
                res.push_back(note);
            }
        }
    }
    
    delete paymentAddress;
    delete spendingKey;
    return res;
}

bool QueryHandler::isValidAuditor(std::string userName)
{
    UserModel *user = Storage::shareInstance()->getSingleUserByName(userName);

    return user && user->getUserType() == ClientTypeAuditor;
}

void QueryHandler::handle()
{
    if (!this->model)
    {
        cout << "输入有误，请重新输入" << endl;
        return;
    }

    if (this->model->getOrders().front() == "X")
    {
        return;
    }

    string userName;
    while (cin >> userName)
    {
        if (!Storage::shareInstance()->getSingleUserByName(userName))
        {
            cout << "没有这个用户，请确认" << endl;
        }

        cout << userName << "的账户余额为" << this->queryUserAccount(userName) << endl;    
    }
}

void QueryHandler::inputInfo()
{
    if (this->model)
    {
        delete this->model;
    }

    string userName;
    cout << "请输入需要查询的用户名" << endl;
    while (cin >> userName)
    {
        if (userName == "X")
        {
            return;
        }
        
        UserModel *user = Storage::shareInstance()->getSingleUserByName(userName);
        if (!user)
        {
            cout << "没有这个用户，请重新输入" << endl;
            continue;
        }
        else if (user)
        {
            break;
        }
    }

    string order;
    this->printHelp();
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

        this->model = new HandlerModel();
        this->model->addOrder(order);
        return;
    }
}

void QueryHandler::printHelp()
{
    cout << "请输入命令" << endl;
    cout << "ACCOUNT 代表查询该账号余额" << endl;
    cout << "HELP 显示帮助" << endl;
    cout << "X 退出" << endl;
}
	
bool QueryHandler::isValidInput(std::string order)
{
    return order == "ACCOUNT" || order == "HELP" || order == "X";
}