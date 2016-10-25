/*
 * vehicleData.h
 *
 *  Created on: Nov 21, 2015
 *      Author: thodoris
 */

#ifndef VEHICLEDATA_H_
#define VEHICLEDATA_H_
#include <string>

class vehicleData {
public:
    vehicleData();
    vehicleData(std::string vehicleId, double chargingPower);
    virtual ~vehicleData();
    std::string vehicleID;
    double chargingPower;
};

#endif /* VEHICLEDATA_H_ */
