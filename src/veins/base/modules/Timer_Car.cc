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
    if ( stage==0){
        traci = TraCIMobilityAccess().get(getParentModule());
    }

    seq = 0;
    spacingLength=0.02; //Spacing in meters/second
    //Nominator is the greatest common divasor of chargingPadLength+spacingLength
    // timeout=spacingLength/traci->getSpeed();
    EV << "Sending initial message\n";
    //1 millisecond timer period
    timeout = 0.001;
    timeoutEvent = new cMessage("timeoutEvent");

    // Generate and send initial message.
    EV << "Sending initial message\n";
    message = generateNewMessage();
    //sendCopyOf(message);
    scheduleAt(simTime()+timeout, timeoutEvent);
}

void Timer_Car::handleMessage(cMessage *msg)
{
    double speed=traci->getSpeed();
    if (speed>1) {
        EV << "EV speed is :"<<speed<<endl;
        sendCopyOf(message);
        timeout= spacingLength/speed;
        scheduleAt(simTime()+timeout, timeoutEvent);
    }
    else {
    scheduleAt(simTime()+0.1, timeoutEvent);
    }
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
