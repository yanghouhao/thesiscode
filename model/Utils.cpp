#include "Utils.h"

#include <serialize.h>
#include <streams.h>
#include <hash.h>
#include <stdint.h>
#include <memory>

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
    bool result = this->cpabe->decrypt(attribute, cipherText, res);
    if (!result)
    {
        res = "decrypt fail";
    }
    
    return res;
}

SproutNotePlaintext Utils::deserializeNote(string note_string)
{
    CDataStream ss(SER_DISK, 0);
    SproutNotePlaintext res;
    ss << note_string;
    ss >> res;
    return res;
}

string Utils::serializeNote(SproutNotePlaintext note_pt)
{
    CDataStream ss(SER_DISK, 0);
    ss << note_pt;
    string res;
    ss >> res;
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