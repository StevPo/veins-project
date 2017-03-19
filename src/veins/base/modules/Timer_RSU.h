/*
 * Timer_RSU.h
 *
 *  Created on: Oct 12, 2016
 *      Author: stavros
 */

#ifndef TIMER_RSU_H
#define TIMER_RSU_H

#include <string.h>
#include <omnetpp.h>
#include "veins/modules/world/annotations/AnnotationManager.h"


using Veins::AnnotationManager;


class Timer_RSU : public cSimpleModule
{
protected:

    cMessage *timeoutEvent;  // holds pointer to the timeout self-message
    int seq;  // message sequence number
    cMessage *message;  // message that has to be re-sent on timeout

protected:
    AnnotationManager* annotations;
    bool sentMessage;
    int out;

public:
    double rsu_timeout;
//    const simtime_t rsu_timeout = 0.1;  // timeout

public:

    Timer_RSU();
    virtual ~Timer_RSU();

    /** @brief Initialization of the module and some variables*/
    virtual void initialize(int stage);

    /** @brief Called every time a message arrives*/
    virtual void handleMessage( cMessage *msg );

  protected:
    virtual cMessage *generateNewMessage();
    virtual void sendCopyOf(cMessage *msg);
};

#endif /* TIMER_RSU_H */
