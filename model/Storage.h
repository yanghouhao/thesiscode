#ifndef _STORAGE_H_
#define _STORAGE_H_

#include <vector>
#include <string>
#include <map>

#include "UserModel.h"
#include "MyBlockChain.h"

class Storage
{
private:
    Storage();
    static Storage *instance;
    std::vector<UserModel *> registedUserArray;
    class CGarbo   //它的唯一工作就是在析构函数中删除CSingleton的实例
	{
	public:
		~CGarbo()
		{
			if(Storage::instance)
            {
                for (auto &&user : instance->registedUserArray)
                {
                    delete user;
                }
                delete instance->storedBlockChain;
                delete Storage::instance;
            }	
		}
	};
	static CGarbo Garbo;

    MyBlockChain *storedBlockChain;
    Ed25519VerificationKey joinSplitPubKey;
public:
    static Storage * shareInstance();
    void addRegistedUser(UserModel *);
    std::vector<UserModel *> getRegistedUserArray();
    UserModel *getSingleUserByName(std::string);
    uint64_t getVpub();
    Ed25519VerificationKey getJSPubKey();

    MyBlockChain *sharedBlockChain();

};

#endif
