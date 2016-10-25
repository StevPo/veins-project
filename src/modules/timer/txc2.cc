//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 2003 Ahmet Sekercioglu
// Copyright (C) 2003-2008 Andras Varga
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

#include <string.h>
#include <omnetpp.h>
#include "modules/mobility/traci/TraCIMobility.h"


using Veins::TraCIMobilityAccess;
using Veins::TraCIMobility;




/**
 * Derive the Txc1 class from cSimpleModule. In the Tictoc1 network,
 * both the `tic' and `toc' modules are Txc1 objects, created by OMNeT++
 * at the beginning of the simulation.
 */
class Txc2 : public cSimpleModule
{
private:
    simtime_t timeout;  // timeout
    cMessage *timeoutEvent;  // holds pointer to the timeout self-message
    int seq;  // message sequence number
    cMessage *message;  // message that has to be re-sent on timeout
    double velocity;
    double chargingPadLength;
    double spacingLength;
    TraCIMobility* traci;

  public:
    Txc2();
    virtual ~Txc2();

  protected:
    virtual cMessage *generateNewMessage();
    virtual void sendCopyOf(cMessage *msg);
    virtual void initialize(int stage);
    virtual void handleMessage(cMessage *msg);
};

// The module class needs to be registered with OMNeT++
Define_Module(Txc2);

Txc2::Txc2()
{
    timeoutEvent = message = NULL;
}

Txc2::~Txc2()
{
    cancelAndDelete(timeoutEvent);
    delete message;
}

void Txc2::initialize(int stage)
{
    // Initialize variables.
    cSimpleModule::initialize(stage);
        if ( stage==0){

       //     traci = TraCIMobilityAccess().get(getParentModule()->getSubmodule("veinsmobility"));

        }

        seq = 0;
        spacingLength=0.2; //Spacing in meters/second
        //Nominator is the greatest common divasor of chargingPadLength+spacingLength
       // timeout=spacingLength/traci->getSpeed();
        EV << "Sending initial message\n";
        //1 millisecond timer period
        timeout = 1;
        timeoutEvent = new cMessage("timeoutEvent");

        // Generate and send initial message.
        EV << "Sending initial message\n";
        message = generateNewMessage();
        sendCopyOf(message);
        scheduleAt(simTime()+timeout, timeoutEvent);


}

void Txc2::handleMessage(cMessage *msg)
{
    // The handleMessage() method is called whenever a message arrives
    // at the module. Here, we just send it to the other module, through
    // gate `out'. Because both `tic' and `toc' does the same, the message
    // will bounce between the two.

   // if (msg==timeoutEvent)
    //   {
           // If we receive the timeout event, that means the packet hasn't
           // arrived in time and we have to re-send it.
           EV << "Timeout expired, resending message and restarting timer\n";
           sendCopyOf(message);
           scheduleAt(simTime()+timeout, timeoutEvent);
      // }

}

cMessage *Txc2::generateNewMessage()
{
    // Generate a message with a different name every time.
    char msgname[20];
    sprintf(msgname, "tic-%d", ++seq);
    cMessage *msg = new cMessage(msgname);
    return msg;
}

void Txc2::sendCopyOf(cMessage *msg)
{
    // Duplicate message and send the copy.
    cMessage *copy = (cMessage *) msg->dup();
    send(copy, "out");
}




