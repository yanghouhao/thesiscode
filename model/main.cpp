#include <iostream>

#include "RegisterHandler.h"
#include "PrintHandler.h"

using namespace std;

int main()
{
    RegisterHandler *registerHandler = RegisterHandler::shareInstance();
    registerHandler->regist("yanghouhao",client);

    PrintHandler *printHandler = PrintHandler::shareInstance();
    printHandler->printAllUser();

    return 0;
}