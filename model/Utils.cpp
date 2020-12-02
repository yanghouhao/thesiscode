#include "Utils.h"

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
    return "";
}

string Utils::decrypt(string attribute, string cipherText)
{
    return "";
}

SproutNotePlaintext Utils::deserializeNote(string)
{
    return SproutNotePlaintext();
}

string Utils::serializeNote(SproutNotePlaintext)
{
    return "";
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