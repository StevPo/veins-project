/*
 * Timer_Car.cc
 *
 *  Created on: Oct 12, 2016
 *      Author: stavros
 */

//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 2003 Ahmet Sekercioglu
// Copyright (C) 2003-2008 Andras Varga
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

#include "veins/base/modules/Timer_Car.h"

using Veins::AnnotationManagerAccess;
using Veins::TraCIMobilityAccess;

// The module class needs to be registered with OMNeT++
Define_Module(Timer_Car);

Timer_Car::Timer_Car()
{
    timeoutEvent = message = NULL;
}

Timer_Car::~Timer_Car()
{
    cancelAndDelete(timeoutEvent);
    delete message;
}

void Timer_Car::initialize(int stage)
{
    cSimpleModule::initialize(stage);

    car_timeout = getParentModule()->par("CarTimer").doubleValue(); //100 millisecond timer period
    timeoutEvent = new cMessage("timeoutEvent");

    // Generate and send initial message.
    EV << "Sending initial message\n";
    message = generateNewMessage();
    scheduleAt(simTime()+simtime_t(car_timeout), timeoutEvent);

}

void Timer_Car::handleMessage(cMessage *msg)
{

      sendCopyOf(message);
      scheduleAt(simTime()+simtime_t(car_timeout), timeoutEvent);

}

cMessage *Timer_Car::generateNewMessage()
{
    char msgname[20];
    sprintf(msgname, "tic-%d", ++seq);
    cMessage *msg = new cMessage(msgname);
    return msg;
}

void Timer_Car::sendCopyOf(cMessage *msg)
{
    cMessage *copy = (cMessage *) msg->dup();
    send(copy, "out");
}
