#include "BaseHandler.h"

void HandlerModel::addOrder(std::string order)
{
    this->orders.push_back(order);
}

std::vector<std::string> HandlerModel::getOrders()
{
    return this->orders;
}