/*
 * vehicleData.cpp
 *
 *  Created on: Nov 21, 2015
 *      Author: thodoris
 */

#include <vehicleData.h>

vehicleData::vehicleData() {
    // TODO Auto-generated constructor stub

}

vehicleData::vehicleData(std::string vegicleId, double chargingPower) {
    // TODO Auto-generated constructor stub
this->chargingPower=chargingPower;
this->vehicleID=vegicleId;
}

vehicleData::~vehicleData() {
    // TODO Auto-generated destructor stub
}

