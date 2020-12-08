#include "Storage.h"
#include "TransactionHandler.h"
#include "MyTransactionPool.h"
#include "Utils.h"
#include "QueryHandler.h"

#include <string>
#include <array>

TransactionHandler * TransactionHandler::instance = nullptr;

TransactionHandler * TransactionHandler::shareInstance()
{
    if (!instance)
    {
        instance = new TransactionHandler();
    }
    return instance;
}

void TransactionHandler::create(string recipientName, int amount)
{
    uint64_t vpub_old = Storage::shareInstance()->getVpub();
    uint64_t vpub_new = vpub_old - amount;

    SproutPaymentAddress * recipientAddress = QueryHandler::shareInstance()->queryUserAddress(recipientName);
    if (!recipientAddress)
    {
        return;
    }
    
    std::array<JSInput, 2> inputs = 
    {
        JSInput(), // dummy input
        JSInput() // dummy input
    };

    std::array<JSOutput, 2> outputs = 
    {
        JSOutput(*recipientAddress, amount),
        JSOutput()// dummy output
    };

    std::array<string, 2> apk_ptArray =
    {
        recipientAddress->a_pk.ToString(),
        "123"
    };

    auto joinSplitPubKey = Storage::shareInstance()->getJSPubKey();

    SproutMerkleTree tree;
    uint256 rt = tree.root();
    std::cout << "开始生成证明" << endl;
    JSDescription *jsdesc = MyTransaction::makeSproutProof(
            inputs,
            outputs,
            joinSplitPubKey,
            vpub_old,
            vpub_new,
            rt
    );
    std::cout << "证明生成完成" << endl;
    
    uint252 phi = random_uint252();
    uint256 h_sig = jsdesc->h_sig(joinSplitPubKey);
    
    std::vector<SproutNotePlaintext> notesTextArray;
    std::array<unsigned char, ZC_MEMO_SIZE> memo;
    std::vector<string> cipherTextArray;
    std::vector<string> addressCipherTextArray;
    string encryptKey = recipientName;
    for (size_t i = 0; i < 2; i++) 
    {
        uint256 r = random_uint256();
        auto note = outputs[i].note(phi, r, i, h_sig);
        
        SproutNotePlaintext note_pt(note, memo);
        string plainText = Utils::shareInstance()->serializeNote(note_pt);
        string cipherText = Utils::shareInstance()->encrypt(encryptKey, plainText); 
        cipherTextArray.push_back(cipherText);

        string addressString = apk_ptArray[i];
        string addressStringCipherText = Utils::shareInstance()->encrypt(encryptKey, addressString);
        addressCipherTextArray.push_back(addressStringCipherText);
    }

    MyTransaction *transaction = new MyTransaction(jsdesc, cipherTextArray, TransactionTypeTreate, addressCipherTextArray);
    this->storeTransaction(transaction, amount);
    std::cout << "交易创建完成" << endl;
    delete recipientAddress;
}

void TransactionHandler::transfer(string publisher, string recipient, int amount, std::vector<SproutNote> noteArray)
{
    QueryHandler *query = QueryHandler::shareInstance();
    if (!query->isAccountMoneyValid(publisher, amount))
    {
        std::cout << publisher << "的账户余额不足，无法进行转账交易" << endl;
        return;
    }

    SproutPaymentAddress * recipientAddress = QueryHandler::shareInstance()->queryUserAddress(recipient);
    if (!recipientAddress)
    {
        return;
    }

    SproutMerkleTree tree;
    JSDescription *jsdesc;
    SproutPaymentAddress * publisherAddress = query->queryUserAddress(publisher);
    SproutSpendingKey * publisherSpendingKey = query->queryUserSpendingKey(publisher);

    auto joinSplitPubKey = Storage::shareInstance()->getJSPubKey();
    std::array<unsigned char, ZC_MEMO_SIZE> memo;

    int transferValue = amount;
    std::vector<string> cipherTextArray;

    string encryptKey = Utils::shareInstance()->encryptKey(publisher, recipient);

    for (auto &&note : noteArray)
    {
        if (transferValue == 0)
        {
            break;
        }
        
        tree.append(note.cm());
        uint252 phi = random_uint252();
        std::array<JSInput, 2> inputs;
        inputs[0] = JSInput();
        inputs[1] = JSInput(tree.witness(), note, *publisherSpendingKey);
        std::array<JSOutput, 2> outputs;
        std::array<string, 2> apk_ptArray;

        int value = note.value();
        if (value < transferValue)
        { 
            outputs[0] =  JSOutput(*recipientAddress, value),
            outputs[1] =  JSOutput();
            apk_ptArray =
            {
                recipientAddress->a_pk.ToString(),
                "123"
            };
            transferValue -= value;       
        }
        else
        {
            outputs[0] =  JSOutput(*recipientAddress, transferValue);
            outputs[1] =  JSOutput(*publisherAddress, value - transferValue);
            apk_ptArray =
            {
                recipientAddress->a_pk.ToString(),
                publisherAddress->a_pk.ToString()
            };
            transferValue = 0;
        }
        std::cout << "开始生成证明" << endl;
        jsdesc = MyTransaction::makeSproutProof(
                                                inputs,
                                                outputs,
                                                joinSplitPubKey,
                                                0,
                                                0,
                                                tree.root()
                                            );

        uint256 h_sig = jsdesc->h_sig(joinSplitPubKey);
        std::vector<string> addressCipherTextArray;
        for (size_t i = 0; i < 2; i++) 
        {
            uint256 r = random_uint256();
            auto note = outputs[i].note(phi, r, i, h_sig);
            SproutNotePlaintext note_pt(note, memo);
            string plainText = Utils::shareInstance()->serializeNote(note_pt);
            string cipherText = Utils::shareInstance()->encrypt(encryptKey, plainText);
            cipherTextArray.push_back(cipherText);

            string addressString = apk_ptArray[i];
            string addressStringCipherText = Utils::shareInstance()->encrypt(encryptKey, addressString);
            addressCipherTextArray.push_back(addressStringCipherText);
        }
        MyTransaction *transaction = new MyTransaction(jsdesc, cipherTextArray, TransactionTypeTransfer, addressCipherTextArray);
        this->storeTransaction(transaction, amount);
    }
    std::cout << "交易创建完成" << endl;
    delete publisherSpendingKey;
    delete jsdesc;
    delete publisherAddress;
}

void TransactionHandler::storeTransaction(MyTransaction *transaction, int amount)
{
    MyTransactionPool *transactionPool = MyTransactionPool::shareInstance();
    int transToPrivate = transaction->type == TransactionTypeTransfer ? 0 : amount;
    transactionPool->addTransaction(transaction, transToPrivate);
}

void TransactionHandler::handle()
{    
    string order = this->model.getOrders().front();
    if (order == "X")
    {
        return;
    } 
    else if (order == "creat")
    {
        string userName;
        int amount;
        std::cout << "请输入用户名" << endl;
        while (std::cin >> userName)
        {
            if (!Storage::shareInstance()->getSingleUserByName(userName))
            {
                std::cout << "没有这个用户，请重新输入" << endl;
                continue;
            }
            break;
        }

        std::cout << "请输入转进金额" << endl;
        std::cin >> amount;

        if (Storage::shareInstance()->getVpub() < amount)
        {
            std::cout << "金额无效，超出透明池范围" << endl;
            return;
        }
        
        this->create(userName, amount);
    }
    else if (order == "transfer")
    {
        string userNameP;
        string userNameR;
        int amount;
        std::cout << "请输入支付用户名" << endl;
        while (std::cin >> userNameP)
        {
            if (userNameP == "X")
            {
                return;
            }
            
            if (!Storage::shareInstance()->getSingleUserByName(userNameP))
            {
                std::cout << "没有这个用户，请重新输入" << endl;
                continue;
            }
            break;
        }

        std::cout << "请输入接受用户名" << endl;
        while (std::cin >> userNameR)
        {
            if (userNameR == "X")
            {
                return;
            }

            if (!Storage::shareInstance()->getSingleUserByName(userNameR))
            {
                std::cout << "没有这个用户，请重新输入" << endl;
                continue;
            }
            break;
        }

        std::cout << "请输入交易金额" << endl;
        std::cin >> amount;

        if (!QueryHandler::shareInstance()->isAccountMoneyValid(userNameP, amount))
        {
            std::cout << "没有足够的金额" << endl;
            return;
        }
        
        std::vector<libzcash::SproutNote> allNote = QueryHandler::shareInstance()->queryUserValidNotesArray(userNameP);
        this->transfer(userNameP, userNameR, amount, allNote);
    }
    
    
}

void TransactionHandler::inputInfo()
{
    this->printHelp();

    string order;

    while (std::cin >> order)
    {
        if (!this->isValidInput(order))
        {
            std::cout << "输入有误，请重新输入" << endl;
            this->printHelp();
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

void TransactionHandler::printHelp()
{
    std::cout << "请输入要执行的交易" << endl;
    std::cout << "creat 代表从透明池向隐私池转账" << endl;
    std::cout << "transfer 代表隐私池之间的转账" << endl;
    //cout << "new 代表新增透明池" << endl;
    std::cout << "HELP 显示帮助" << endl;
    std::cout << "X 退出" << endl;
}
	
bool TransactionHandler::isValidInput(std::string order)
{
    return order == "new" || order == "creat" || order == "HELP" || order == "transfer" || order == "X";
}