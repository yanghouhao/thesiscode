#ifndef _UTILS_H_
#define _UTILS_H_

#include <openabe/openabe.h>
#include <openabe/zsymcrypto.h>
#include <time.h>

#include "zcash/JoinSplit.hpp"
#include "zcash/Note.hpp"

using namespace oabe;
using namespace oabe::crypto;
using namespace std;
using namespace libzcash;

enum AESKeyLength
{
    AES_KEY_LENGTH_16 = 16, AES_KEY_LENGTH_24 = 24, AES_KEY_LENGTH_32 = 32 
};

class Utils
{
private:
    Utils(){};
    static OpenABECryptoContext *cpabe;
    static Utils *instance;
    class CGarbo   //它的唯一工作就是在析构函数中删除CSingleton的实例
	{
	public:
		~CGarbo()
		{
            if (Utils::egroup)
            {
                delete Utils::egroup;
            }

            if (Utils::cpabe)
            {
                delete Utils::cpabe;
            }
            
			if(Utils::instance)
            {
                delete Utils::instance;
            }

            if (Utils::rng)
            {
                delete Utils::rng;
            }
            	
		}
	};
    static OpenABEEllipticCurve *egroup;
    static OpenABERNG *rng;
public:
    static Utils * shareInstance();
    string encryptCPABE(string, string);
    //密钥在前，明/密文在后
    string decryptCPABE(string, string);
    SproutNotePlaintext deserializeNote(string);
    string serializeNote(SproutNotePlaintext);
    string encryptKey(string, string);

    vector<clock_t> testEncTime(string, string, string);

    std::string aes_256_cbc_decode(const std::string& password, const std::string& strData);
    std::string aes_256_cbc_encode(const std::string& password, const std::string& data);

    G_t getGenerator();
    ZP_t getRandomNumber();
    ZP_t hashGt2Zpt(G_t);
};

#endif