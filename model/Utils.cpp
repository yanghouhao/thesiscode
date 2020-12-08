#include "Utils.h"

#include <serialize.h>
#include <streams.h>
#include <hash.h>
#include <stdint.h>
#include <memory>
#include <vector>

Utils * Utils::instance = nullptr;
OpenABECryptoContext * Utils::cpabe = nullptr;

Utils * Utils::shareInstance()
{
    if (!instance)
    {
        instance = new Utils();
        if (!cpabe)
        {
            InitializeOpenABE();
            cpabe = new OpenABECryptoContext("CP-ABE");
            cpabe->generateParams();
        }
    }
    return instance;
}

string Utils::encrypt(string policy, string plainText)
{
    string res;
    this->cpabe->encrypt(policy, plainText, res);
    return res;
}

string Utils::decrypt(string attribute, string cipherText)
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

/*
InitializeOpenABE();

  cout << "Testing CP-ABE context" << endl;

  OpenABECryptoContext cpabe("CP-ABE");

  string ct, pt1 = "hello world!", pt2;

  cpabe.generateParams();

  cpabe.keygen("attr1|attr2", "key0");

  cpabe.encrypt("attr1 and attr2", pt1, ct);

  bool result = cpabe.decrypt("key0", ct, pt2);

  cout << pt2 << endl;

  //assert(result && pt1 == pt2);

  cout << "Recovered message: " << pt2 << endl;

  ShutdownOpenABE();
  */