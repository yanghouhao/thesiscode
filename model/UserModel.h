#ifndef _USERMODEL_H_
#define _USERMODEL_H_

#include <string>
#include <vector>
#include "zcash/JoinSplit.hpp"
#include "zcash/Note.hpp"

using namespace libzcash;

enum ClientType {
    client,
    auditor,
    creator,
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
    std::string getAttribute();
    std::vector<SproutNote> getValidNotesArray();
};

#endif
