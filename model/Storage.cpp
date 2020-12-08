#include "Storage.h"

Storage::Storage()
{
    storedBlockChain = new MyBlockChain();
    GetRandBytes(this->joinSplitPubKey.bytes, ED25519_VERIFICATION_KEY_LEN);
}

Storage * Storage::instance = nullptr;

Storage * Storage::shareInstance()
{
    if (!instance)
    {
        instance = new Storage();
        instance->storedBlockChain = new MyBlockChain();
    }
    return instance;
}

void Storage::addRegistedUser(UserModel *user)
{
    this->registedUserArray.push_back(user);
}

std::vector<UserModel *> Storage::getRegistedUserArray()
{
    return Storage::shareInstance()->registedUserArray;
}

uint64_t Storage::getVpub()
{
    return this->storedBlockChain->getVpub();
}

UserModel * Storage::getSingleUserByName(std:: string userName)
{
    for (auto &&userModel : this->registedUserArray)
    {
        if (userModel->getName() == userName)
        {
            return userModel;
        } 
    }
    return nullptr;
}

Ed25519VerificationKey Storage::getJSPubKey()
{
    return this->joinSplitPubKey;
}

MyBlockChain* Storage::sharedBlockChain()
{
    return this->storedBlockChain;
}