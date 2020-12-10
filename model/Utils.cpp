#include "Utils.h"

#include <serialize.h>
#include <streams.h>
#include <hash.h>
#include <stdint.h>
#include <memory>
#include <vector>

#include <openssl/aes.h>

Utils * Utils::instance = nullptr;
OpenABECryptoContext * Utils::cpabe = nullptr;
OpenABEEllipticCurve * Utils::egroup = nullptr;
OpenABERNG * Utils::rng = nullptr;

Utils * Utils::shareInstance()
{
    if (!instance)
    {
        instance = new Utils();
        if (!cpabe)
        {
            cpabe = new OpenABECryptoContext("CP-ABE");
            cpabe->generateParams();
        }

        if (!egroup)
        {
            egroup = new OpenABEEllipticCurve(DEFAULT_EC_PARAM);
        }

        if (!rng)
        {
            rng = new OpenABERNG();
        }
        
        
    }
    return instance;
}

string Utils::encryptCPABE(string policy, string plainText)
{
    string res;
    this->cpabe->encrypt(policy, plainText, res);
    return res;
}

string Utils::decryptCPABE(string attribute, string cipherText)
{
    string res;
    this->cpabe->keygen(attribute, "key0");
    bool result = this->cpabe->decrypt("key0", cipherText, res);
    if (!result)
    {
        res = "decrypt fail";
    }
    
    return res;
}

SproutNotePlaintext Utils::deserializeNote(string note_string)
{
    CDataStream ss(SER_NETWORK, 0);
    ss.clear();
    SproutNotePlaintext res;
    ss.write(note_string.c_str(), note_string.size());
    ss >> res;
    return res;
}

string Utils::serializeNote(SproutNotePlaintext note_pt)
{
    CDataStream ss(SER_NETWORK, 0);
    ss.clear();
    ss << note_pt;
    string res = ss.str();
    ss.clear();
    return res;
}

string Utils::encryptKey(string publisherName, string recipientName)
{
    string res;
    res = publisherName + " or " + recipientName;
    return res;
}

vector<clock_t> Utils::testEncTime(string policy, string plainText, string p)
{
    string res;
    string test;
    auto start = clock();
    this->cpabe->encrypt(policy, plainText, res);
    auto end = clock();
    this->cpabe->keygen(p, "key0");
    auto startEnc = clock();
    bool result = this->cpabe->decrypt("key0", res, test);
    auto endEnc = clock();
    vector<clock_t> resv;
    resv.push_back(end - start);
    resv.push_back(endEnc - startEnc);
    return resv;
}

std::string Utils::aes_256_cbc_encode(const std::string& password, const std::string& data)
{
	// 这里默认将iv全置为字符0
	unsigned char iv[AES_BLOCK_SIZE] = { '0','0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0' };

	AES_KEY aes_key;
	if (AES_set_encrypt_key((const unsigned char*)password.c_str(), password.length() * 8, &aes_key) < 0)
	{
		//assert(false);
		return "";
	}
	std::string strRet;
	std::string data_bak = data;
	unsigned int data_length = data_bak.length();

	// ZeroPadding
	int padding = 0;
	if (data_bak.length() % (AES_BLOCK_SIZE) > 0)
	{
		padding = AES_BLOCK_SIZE - data_bak.length() % (AES_BLOCK_SIZE);
	}
	// 在一些软件实现中，即使是16的倍数也进行了16长度的补齐
	/*else
	{
		padding = AES_BLOCK_SIZE;
	}*/
	
	data_length += padding;
	while (padding > 0)
	{
		data_bak += '\0';
		padding--;
	}

	for (unsigned int i = 0; i < data_length / (AES_BLOCK_SIZE); i++)
	{
		std::string str16 = data_bak.substr(i*AES_BLOCK_SIZE, AES_BLOCK_SIZE);
		unsigned char out[AES_BLOCK_SIZE];
		::memset(out, 0, AES_BLOCK_SIZE);
		AES_cbc_encrypt((const unsigned char*)str16.c_str(), out, AES_BLOCK_SIZE, &aes_key, iv, AES_ENCRYPT);
		strRet += std::string((const char*)out, AES_BLOCK_SIZE);
	}
	return strRet;
}

std::string Utils::aes_256_cbc_decode(const std::string& password, const std::string& strData)
{
	// 这里默认将iv全置为字符0
	unsigned char iv[AES_BLOCK_SIZE] = { '0','0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0' };

	AES_KEY aes_key;
	if (AES_set_decrypt_key((const unsigned char*)password.c_str(), password.length() * 8, &aes_key) < 0)
	{
		//assert(false);
		return "";
	}
	std::string strRet;
	for (unsigned int i = 0; i < strData.length() / AES_BLOCK_SIZE; i++)
	{
		std::string str16 = strData.substr(i*AES_BLOCK_SIZE, AES_BLOCK_SIZE);
		unsigned char out[AES_BLOCK_SIZE];
		::memset(out, 0, AES_BLOCK_SIZE);
		AES_cbc_encrypt((const unsigned char*)str16.c_str(), out, AES_BLOCK_SIZE, &aes_key, iv, AES_DECRYPT);
		strRet += std::string((const char*)out, AES_BLOCK_SIZE);
	}
	return strRet;
}

G_t Utils::getGenerator()
{
    return egroup->getGenerator();
}

ZP_t Utils::getRandomNumber()
{
    return egroup->randomZP(rng);
}

ZP_t Utils::hashGt2Zpt(G_t gt)
{
    OpenABEByteString serializeContext;
    gt.serialize(serializeContext);
    string gtStr = serializeContext.toString();
    std::hash<string> hashContext;
    size_t hashed = hashContext(gtStr);
    string hashedStr = to_string(hashed);
    char *st1 = const_cast<char *>(hashedStr.c_str());
    return ZP_t(st1);
}
