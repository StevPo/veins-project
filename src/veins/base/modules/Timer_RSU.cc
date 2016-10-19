/*
 * Timer_RSU.cc
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

#include "veins/base/modules/Timer_RSU.h"

using Veins::AnnotationManagerAccess;

// The module class needs to be registered with OMNeT++
Define_Module(Timer_RSU);

Timer_RSU::Timer_RSU()
{
    timeoutEvent = message = NULL;
}

Timer_RSU::~Timer_RSU()
{
    cancelAndDelete(timeoutEvent);
    delete message;
}

void Timer_RSU::initialize(int stage)
{
    cSimpleModule::initialize(stage);

    //100 millisecond timer period
    timeout = 0.1;
    timeoutEvent = new cMessage("timeoutEvent");

    // Generate and send initial message.
    EV << "Sending initial message\n";
    message = generateNewMessage();
    scheduleAt(simTime()+timeout, timeoutEvent);
}

void Timer_RSU::handleMessage(cMessage *msg)
{
    sendCopyOf(message);
    scheduleAt(simTime()+timeout, timeoutEvent);
}

cMessage *Timer_RSU::generateNewMessage()
{
    char msgname[20];
    sprintf(msgname, "toc-%d", ++seq);
    cMessage *msg = new cMessage(msgname);
    return msg;
}

void Timer_RSU::sendCopyOf(cMessage *msg)
{
    cMessage *copy = (cMessage *) msg->dup();
    send(copy, "out");
}
