/*
 * PowerDemand.h
 *
 *  Created on: Feb 1, 2016
 *      Author: theodoros
 */

#ifndef POWERDEMAND_H_
#define POWERDEMAND_H_


class PowerDemand {
public:
    PowerDemand();
    virtual ~PowerDemand();
    static double demand;
    static double cost;
    static double storage;
    double getDemand(void);
    double addDemand(double);
    double removeDemand(double);
    double getStorage(void);
    double addStorage(double);
    double removeStorage(double);
    double removeCost(double);
    double addCost(double);
    double getCost(void);


};



#endif /* POWERDEMAND_H_ */
