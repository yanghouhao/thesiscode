#ifndef _UTILS_H_
#define _UTILS_H_

#include <openabe/openabe.h>
#include <openabe/zsymcrypto.h>

#include "zcash/JoinSplit.hpp"
#include "zcash/Note.hpp"

using namespace oabe;
using namespace oabe::crypto;
using namespace std;
using namespace libzcash;

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
			if(Utils::instance)
            {
                ShutdownOpenABE();
                delete Utils::cpabe;
                delete Utils::instance;
            }	
		}
	};
public:
    static Utils * shareInstance();
    string encrypt(string, string);
    string decrypt(string, string);
    SproutNotePlaintext deserializeNote(string);
    string serializeNote(SproutNotePlaintext);
};

#endif