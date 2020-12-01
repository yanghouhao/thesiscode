#include "TransactionHandler.h"
#include "model/MyTransaction.h"

#include "Poco/JSON/Parser.h"
#include "Poco/JSON/ParseHandler.h"
#include "Poco/JSON/Stringifier.h"

void TransactionHandler::handleRequest(HTTPServerRequest &req, HTTPServerResponse &resp)
{
    resp.setStatus(HTTPResponse::HTTP_OK);
    resp.setContentType("text/html");

    ostream & out = resp.send();

    out << "<h1>Hello World!</h1>";
}

MyTransaction * deserializeFromJSON(string &jsonString)
{
    // Parse the JSON
    Poco::JSON::Parser jsonParser;
    Poco::Dynamic::Var parsedJSON = jsonParser.parse(jsonString);
    Poco::Dynamic::Var parsedResult = jsonParser.result();
    
    MyTransaction *transaction = new MyTransaction;

    // Extract top-level fields
    Poco::DynamicStruct jsonStruct = *parsedResult.extract<Poco::JSON::Object::Ptr>();
    string cipherText = jsonStruct["cipher_text"].toString();

    // Get metadata nested fields
    string keyStr = "JSDescription";
    Poco::JSON::Object::Ptr jsonObject = parsedResult.extract<Poco::JSON::Object::Ptr>();
    Poco::Dynamic::Var metaVar = jsonObject->get(keyStr);
    Poco::JSON::Object::Ptr metaObj = metaVar.extract<Poco::JSON::Object::Ptr>();

    JSDescription *jsDescription = new JSDescription();
    for (Poco::JSON::Object::ConstIterator itr = metaObj->begin(), end = metaObj->end(); itr != end; ++itr) {
        if (itr->first == "vpub_old") {
            jsDescription->vpub_old = (CAmount)stoi(itr->second.toString());
        } else if (itr->first == "vpub_new") {
            jsDescription->vpub_new = (CAmount)stoi(itr->second.toString());
        } else if (itr->first == "anchor") {
            uint256 rv;
            rv.SetHex(itr->second.toString());
            jsDescription->anchor = rv;
        } else if (itr->first == "nullifiers") {

        }
        
    }

    return transaction;
}

/*
this->jsDescription = new JSDescription;
    this->jsDescription->vpub_old = jsDescription->vpub_old ;
    this->jsDescription->vpub_new = jsDescription->vpub_new ;
    this->jsDescription->anchor = jsDescription->anchor ;
    this->jsDescription->nullifiers = jsDescription->nullifiers ;
    this->jsDescription->commitments = jsDescription->commitments ;
    this->jsDescription->ephemeralKey = jsDescription->ephemeralKey ;
    this->jsDescription->ciphertexts = jsDescription->ciphertexts ;
    this->jsDescription->randomSeed = jsDescription->randomSeed ;
    this->jsDescription->macs = jsDescription->macs ;
    this->jsDescription->proof = jsDescription->proof;

    this->cipherTextWithABE = cipherTextWithABE;
*/