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
    //密钥在前，明/密文在后
    string decrypt(string, string);
    SproutNotePlaintext deserializeNote(string);
    string serializeNote(SproutNotePlaintext);
    string encryptKey(string, string);
};

template<class T, size_t N, typename V>
std::array<T, N> to_array(V v)
{
    assert(v.size() == N);
    std::array<T, N> d;
    using std::begin; using std::end; 
    std::copy( begin(v), end(v), begin(d) ); // this is the recommended way
    return d;
}

#endif