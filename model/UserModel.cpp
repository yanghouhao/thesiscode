#include "UserModel.h"
#include "Utils.h"

#include "string"

UserKeyContext::UserKeyContext(/* args */)
{
    this->privateKey = Utils::shareInstance()->getRandomNumber();
    G_t generator = Utils::shareInstance()->getGenerator();
    this->publicKey = generator.exp(this->privateKey);
}

UserKeyContext::~UserKeyContext()
{
}

G_t UserKeyContext::getPublicKey()
{
    return this->publicKey;
}

ZP_t UserKeyContext::getPrivateKey()
{
    return this->privateKey;
}

UserModel::UserModel(std::string userName, ClientType userType = ClientTypeClient)
{
    this->userName = userName;
    this->userType = userType;
    this->spendingKey = SproutSpendingKey::random();
    this->address = this->spendingKey.address();
}

UserModel::UserModel(UserModel *user)
{
    this->userName = user->userName;
    this->userType = user->userType;
    this->auditeeArray = user->auditeeArray;
    this->spendingKey = user->spendingKey;
    this->address = user->address;
    
}

UserModel::~UserModel()
{
    for (auto &&user : this->auditeeArray)
    {
        delete user;
    }
}

void UserModel::addAuditee(UserModel *auditee)
{
    UserModel *user = new UserModel(auditee);
    this->auditeeArray.push_back(user);
}

std::vector<UserModel *> UserModel::getAuditeeArray()
{
    return this->auditeeArray;
}

void UserModel::resetAuditeeArray(std::vector<UserModel *>auditeeArray)
{
    for (auto &&user : this->auditeeArray)
    {
        delete user;
    }
    this->auditeeArray = auditeeArray;
}

std::string UserModel::getName()
{
    return this->userName;
}

ClientType UserModel::getUserType()
{
    return this->userType;
}

SproutPaymentAddress UserModel::getPaymentAddress()
{
    return this->address;
}

SproutSpendingKey UserModel::getSpendingKey()
{
    return this->spendingKey;
}

std::string UserModel::getAttribute()
{
    std::string res;
    for (auto &&auditee : this->auditeeArray)
    {
        res += "|";
        res += auditee->getName();
    }
    return res;
}

std::vector<SproutNote> UserModel::getValidNotesArray()
{
    return this->validNotesArray;
}