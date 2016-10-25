/*
 * Timer_Car.h
 *
 *  Created on: Oct 12, 2016
 *      Author: stavros
 */

#ifndef TIMER_CAR_H
#define TIMER_CAR_H

#include <string.h>
#include <omnetpp.h>
#include "veins/modules/world/annotations/AnnotationManager.h"
#include "veins/modules/mobility/traci/TraCIMobility.h"

using Veins::TraCIMobility;
using Veins::AnnotationManager;


class Timer_Car : public cSimpleModule
{
protected:

    cMessage *timeoutEvent;  // holds pointer to the timeout self-message
    int seq;  // message sequence number
    cMessage *message;  // message that has to be re-sent on timeout
    double chargingPadLength;
    double spacingLength;
    TraCIMobility* traci;

protected:
    AnnotationManager* annotations;
    bool sentMessage;
    int out;

public:
    const simtime_t car_timeout = 0.001;  // timeout

public:

    Timer_Car();
    virtual ~Timer_Car();

    /** @brief Initialization of the module and some variables*/
    virtual void initialize(int stage);

    /** @brief Called every time a message arrives*/
    virtual void handleMessage( cMessage *msg );

  protected:
    virtual cMessage *generateNewMessage();
    virtual void sendCopyOf(cMessage *msg);
};


#endif /* TIMER_CAR_H */
