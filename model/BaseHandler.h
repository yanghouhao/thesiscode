#ifndef _BASEHANDLER_H_
#define _BASEHANDLER_H_

#include <vector>
#include <string>

class HandlerModel
{
private:
    std::vector<std::string> orders;
public:
    HandlerModel(std::vector<std::string> orders){
        this->orders = orders;
    };
    HandlerModel(){};
    void addOrder(std::string);
    std::vector<std::string> getOrders();
};

class BaseHandler
{
private:
    
public:
    HandlerModel * model;
    virtual void handle() = 0;
    virtual void inputInfo() = 0;
    virtual void printHelp() = 0;
    virtual bool isValidInput(std::string) = 0;
};

#endif