#ifndef _USERMODEL_H_
#define _USERMODEL_H_

#include "zcash/JoinSplit.hpp"
#include "zcash/Note.hpp"

#include <string>
#include <vector>
#include <openabe/openabe.h>
#include <openabe/zsymcrypto.h>
using namespace oabe;
using namespace oabe::crypto;

using namespace libzcash;

enum ClientType {
    ClientTypeClient,
    ClientTypeAuditor,
    ClientTypeCreator,
};

class UserKeyContext
{
private:
    G_t publicKey;
    ZP_t privateKey;
public:
    UserKeyContext(/* args */);
    ~UserKeyContext();
    G_t getPublicKey();
    ZP_t getPrivateKey();
};

class UserModel
{
private:
    std::string userName;
    ClientType userType;
    std::vector<UserModel *> auditeeArray;
    SproutSpendingKey spendingKey;
    SproutPaymentAddress address;
    std::vector<SproutNote> validNotesArray;
    UserKeyContext UserKaypair;
public:
    UserModel(std::string, ClientType);
    UserModel(UserModel *);
    ~UserModel();
    void addAuditee(UserModel *);
    void resetAuditeeArray(std::vector<UserModel *>);
    std::vector<UserModel *> getAuditeeArray();
    std::string getName();
    ClientType getUserType();
    SproutPaymentAddress getPaymentAddress();
    SproutSpendingKey getSpendingKey();
    std::string getAttribute();
    std::vector<SproutNote> getValidNotesArray();
};

#endif
