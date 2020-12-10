#include "AuditContent.h"
#include "Utils.h"

CommitmentContent::CommitmentContent(ZP_t v, ZP_t sk)
{
    auto g = Utils::shareInstance()->getGenerator();
    this->V = g.exp(v);
    auto c = this->getChallenge();
    this->r = v - sk * c;
}

ZP_t CommitmentContent::getChallenge()
{
    return Utils::shareInstance()->hashGt2Zpt(this->V);
}

bool CommitmentContent::verify(G_t pk)
{
    auto g = Utils::shareInstance()->getGenerator();
    auto c = this->getChallenge();
    auto V1 = g.exp(this->r);  
    auto V2 = pk.exp(c);
    return this->V == V1 * V2;
}

CipherTextContent::CipherTextContent(string key, string plainText)
{
    this->cipherText = Utils::shareInstance()->encryptCPABE(key, plainText);
}

string CipherTextContent::decString(string key)
{
    return Utils::shareInstance()->decryptCPABE(key, this->cipherText);
}