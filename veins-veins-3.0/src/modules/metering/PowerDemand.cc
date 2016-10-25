/*
 * PowerDemand.cpp
 *
 *  Created on: Feb 1, 2016
 *      Author: theodoros
 */

#include <PowerDemand.h>


PowerDemand::PowerDemand() {
    // TODO Auto-generated constructor stub

}

PowerDemand::~PowerDemand() {
    // TODO Auto-generated destructor stub
}


double PowerDemand::demand = 0.0;
double PowerDemand::storage = 0.0;

double PowerDemand::cost = 0.0;

double PowerDemand::getDemand(void) {
    // TODO Auto-generated destructor stub
    return demand;
}

double PowerDemand::addDemand(double demandInput) {
    // TODO Auto-generated destructor stub
    demand+=demandInput;
    return demand;
}

double PowerDemand::removeDemand(double demandInput) {
    // TODO Auto-generated destructor stub
    demand-=demandInput;
    return demand;
}

double PowerDemand::addStorage(double demandInput) {
    // TODO Auto-generated destructor stub
    storage+=demandInput;
    return storage;
}

double PowerDemand::removeStorage(double demandInput) {
    // TODO Auto-generated destructor stub
    storage-=demandInput;
    return storage;
}

double PowerDemand::getStorage() {
    // TODO Auto-generated destructor stub

    return storage;
}


double PowerDemand::getCost() {
    // TODO Auto-generated destructor stub

    return cost;
}

double PowerDemand::addCost(double costInput) {
    // TODO Auto-generated destructor stub
    cost+=costInput;
    return cost;
}

double PowerDemand::removeCost(double costInput) {
    // TODO Auto-generated destructor stub
    cost-=costInput;
    return cost;
}



