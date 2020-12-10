#include "TestHandler.h"
#include "Storage.h"
#include "UserModel.h"
#include "Utils.h"
#include "TransactionHandler.h"
#include "QueryHandler.h"
#include "string"

#include <fstream>
#include <streambuf>
#include <time.h>
#include <openssl/des.h>
#include <openabe/openabe.h>
#include <openabe/zsymcrypto.h>

void attrubuteNumberExperiment(int n);

using namespace std;
using namespace oabe;
using namespace oabe::crypto;

const int times = 1;
ofstream oFile;

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
    string ct = Utils::shareInstance()->encryptCPABE(client->getName(),pt);
    cout << "ciphertext is :" << ct << endl;

    string pt1 = Utils::shareInstance()->decryptCPABE(auditor->getAttribute(), ct);
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
        auto plaintText = Utils::shareInstance()->decryptCPABE("yangc", cipherText);
        
        auto note = Utils::shareInstance()->deserializeNote(plaintText);
    }
}

string generateKB()
{
    string res;
    for (size_t i = 0; i < 1024; i++)
    {
        srand(time(NULL));
        switch ((rand() % 3))
        {
        case 1:
            res += ('A' + rand() % 26);
            break;
        case 2:
            res += ('a' + rand() % 26);
            break;
        default:
            res += ('0' + rand() % 10);
            break;
        }
    }
    return res;
}

string generateMB()
{
    string res;
    for (size_t i = 0; i < 1024; i++)
    {
        res += generateKB();    
    }
    return res;
}

string generateTenByte()
{
    string res;
    for (size_t i = 0; i < 10; i++)
    {
        srand(time(NULL));
        switch ((rand() % 3))
        {
        case 1:
            res += ('A' + rand() % 26);
            break;
        case 2:
            res += ('a' + rand() % 26);
            break;
        default:
            res += ('0' + rand() % 10);
            break;
        }
    }
    return res;
}

string generateNByte(size_t n)
{
    srand((unsigned)time(NULL));
    string res;
    for (size_t i = 0; i < n; i++)
    {
        switch ((rand() % 2))
        {
        case 1:
            res += ('A' + rand() % 26);
            break;
        default:
            res += ('a' + rand() % 26);
            break;
        //default:
        //    res += ('0' + rand() % 10);
        //    break;
        }
    }
    return res;
}


void testCPABE1()
{
    vector<int> numberOfAttributes = {
        2, 5, 12, 20, 50, 100, 500
    };
    oFile.open("data.csv", ios::out | ios::trunc);
    oFile << "属性个数" << "," << "属性长度" << "," << "明文长度" << "," << "使用时间ms" << "," << "解密时间" << endl;
    for (auto &&number : numberOfAttributes)
    {
        for (size_t i = 0; i < times; i++)
        {
            attrubuteNumberExperiment(number);
        } 
    }
    
}

void attrubuteNumberExperiment(int n)
{
    vector<int> policyLenth = {
        64,
        128,
        256,
        512,
    };

    vector<size_t> msgSize = {
        10,
        1024,
        1048576,
    };

    vector<string> msgS = {
        "10Byte",
        "1KB",
        "1MB"
    };

    vector<string> msgArray;
    for (size_t i = 0; i < msgSize.size(); i++)
    {
        string str = generateNByte(msgSize[i]);
        msgArray.push_back(str);
    }

    for (size_t t = 0; t < times; t++)
    {
        for (size_t i = 0; i < policyLenth.size(); i++)
        {
            string policyString;
            string policy;
            for (size_t j = 0; j < n; j++)
            {
                string str = generateNByte(policyLenth[i]);
                if (j == 0)
                {
                    policy = str;
                }
                
                policyString += str;
                if (j != n - 1)
                {
                    policyString += " or ";
                }
            }
            
            for (size_t j = 0; j < msgArray.size(); j++)
            {
                string msg = msgArray[j];
                //cout << msg << endl;
                //cout << policyString << endl;
                clock_t allTime = 0;
                clock_t decTime = 0;
                int timeEx = 100;
                for (size_t tt = 0; tt < timeEx; tt++)
                {
                    auto vect = Utils::shareInstance()->testEncTime(policyString, msg, policy);
                    allTime += vect[0];
                    decTime += vect[1];
                }
                //double consumeTime = allTime / (double)timeEx;
                //auto consumeTime = Utils::shareInstance()->testEncTime(policyString, msg);
                //cout << "第" << t << "次实验" << endl;
                //cout << "属性数量为 : " << n << endl;
                //cout << "属性长度为 : " << policyLenth[i] << endl;
                //cout << "明文长度为 : " << msgS[j] << endl;
                allTime /= 100;
                decTime /= 100;
                cout << "所用时间为 : " << ((double)allTime) / 1000 << "ms" << endl;
                cout << "解密时间为 : " << ((double)decTime) / 1000 << "ms" << endl;
                cout << "------------------" << endl;
                oFile << n << "," << policyLenth[i] << "," << msgS[j] << "," << ((double)allTime) / 1000 << "," << ((double)decTime) / 1000 << endl;
            }
        }
    }
    
    
}

void testCreatNote()
{
    cout << "开始测试" << endl;
    oFile.open("creatNote.csv", ios::out | ios::trunc);
    oFile << "生成证明时间ms" << "," << "加密时间ms" << "," << "验证证明时间ms" << "," << "解密时间ms" << "," << "交易大小" << endl;
    size_t times = 100;
    for (size_t i = 0; i < times; i++)
    {
        cout << "开始第次测试 ： " << i + 1 << endl;
        auto timeArray = TransactionHandler::shareInstance()->testCreatNote();
        oFile << timeArray[0] << "," << timeArray[1] << "," << timeArray[2] << "," << timeArray[3] << "," << timeArray[4] << endl;

    }
    cout << "测试结束" << endl;
}

void testTransferNote()
{
    cout << "开始测试" << endl;
    oFile.open("creatNote.csv", ios::out | ios::trunc);
    oFile << "生成证明时间ms" << "," << "加密时间ms" << "," << "验证证明时间ms" << "," << "解密时间ms" << endl;
    size_t times = 100;
    for (size_t i = 0; i < times; i++)
    {
        cout << "开始第次测试 ： " << i + 1 << endl;
        auto timeArray = TransactionHandler::shareInstance()->testCreatNote();
        oFile << timeArray[0] << "," << timeArray[1] << "," << timeArray[2] << "," << timeArray[3] << endl;

    }
    cout << "测试结束" << endl;
}

void testManyNote()
{
    TransactionHandler::shareInstance()->create("yangc", 10);
    TransactionHandler::shareInstance()->create("yangc", 15);
    auto noteArray = QueryHandler::shareInstance()->queryUserValidNotesArray("yangc");
    TransactionHandler::shareInstance()->transfer("yangc", "yanghouhao", 20, noteArray);
}

void testECC()
{
    hash<string> ha;
    size_t mudamudamuda = ha("muda");
    string test = to_string(mudamudamuda);
    cout << test << endl;
    char *st1 = const_cast<char *>(test.c_str());
    ZP_t myzp = ZP_t(st1); 
    
    OpenABE_ERROR err_code = OpenABE_NOERROR;
    OpenABEEllipticCurve egroup(DEFAULT_EC_PARAM);
    OpenABERNG rng;
    OpenABEByteString result;

    G_t h = egroup.getGenerator();
    cout << "(ECGroup) h : " << h << endl;

    ZP_t a = egroup.randomZP(&rng);
    G_t A = h.exp(a);
    
    cout << "A : " << A << endl;
    result.clear();
    A.serialize(result);
    cout << "A bytes: " << result.toLowerHex() << endl;

    bignum_t order;
    egroup.getGroupOrder(order);
    myzp.setOrder(order);
    G_t B = h.exp(myzp);
    cout << "B : " <<B <<endl;

}

void testAES()
{
    OpenABEEllipticCurve egroup(DEFAULT_EC_PARAM);
    OpenABERNG rng;
    ZP_t a = Utils::shareInstance()->getRandomNumber();
    OpenABEByteString result;
    a.serialize(result);
    string plaintText = result.toString();
    cout << "plainText : " << plaintText << " " << "plaintext size" << plaintText.size() << endl;
    string encKey = "12345678901234561234567890123456";
    string cipherText = Utils::shareInstance()->aes_256_cbc_encode(encKey, plaintText);
    
    string decString = Utils::shareInstance()->aes_256_cbc_decode(encKey, cipherText);
    cout << "decString : " << decString << " " << "dec size" << decString.size() << endl;

    string withoutAfter = string(decString.begin(), decString.begin() + 35);
    cout << withoutAfter << " " << withoutAfter.size() << endl;

    char *st1 = const_cast<char *>(decString.c_str());
    cout << a << endl;
    OpenABEByteString tetete;  
    tetete.fromString(withoutAfter);
    ZP_t myzp;
    myzp.deserialize(tetete);
    bignum_t order;
    egroup.getGroupOrder(order);
    myzp.setOrder(order);
    cout << myzp << endl;
}

void TestHandler::handle()
{
    //this->testUtils();
    //testCreat();
    //testCPABE1();
    //testCreatNote();
    //testTransferNote();
    //testManyNote();
    //testECC();
    testAES();
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
