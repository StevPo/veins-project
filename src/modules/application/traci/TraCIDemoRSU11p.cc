//
// Copyright (C) 2006-2011 Christoph Sommer <christoph.sommer@uibk.ac.at>
//
// Documentation for these modules is at http://veins.car2x.org/
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//

#include "application/traci/TraCIDemoRSU11p.h"
#include "PowerDemand.h"

using Veins::AnnotationManagerAccess;

Define_Module(TraCIDemoRSU11p);

void TraCIDemoRSU11p::initialize(int stage) {

	BaseWaveApplLayer::initialize(stage);
	totalChargingLevel=0;
    vehicledata=std::vector<vehicleData>();
	if (stage == 0) {
	    mobi = dynamic_cast<BaseMobility*> (getParentModule()->getSubmodule("mobility"));
	    ASSERT(mobi);
	    annotations = AnnotationManagerAccess().getIfExists();
	    ASSERT(annotations);
	    costVector.setName("Cost vector");
	}
    this->demand=PowerDemand();

	tickTocks=0;
	supply=2000.0;
}

void TraCIDemoRSU11p::onBeacon(WaveShortMessage* wsm) {

}

void TraCIDemoRSU11p::finish() {

    std::cout << "Hop count, min:    " <<  utilisationStats.getMin()<< endl;
    std::cout << "Hop count, max:    " <<  utilisationStats.getMax() << endl;
    std::cout << "Hop count, mean:   " <<  utilisationStats.getMean() << endl;
    std::cout << "Hop count, std:   " <<  utilisationStats.getStddev() << endl;
    std::cout << "Cost is" <<  costStats.getMean()<< endl;


    utilisationStats.recordAs("Utilisation stats");

}


void TraCIDemoRSU11p::onTimer(cMessage* msg)
{
    tickTocks++;
    //double supply;
    double utilisation;
    //record the demand through the monitoring of the static metering value
    PowerDemand demand=PowerDemand();
    totalChargingLevel=demand.getDemand();
    //printout the overall demand
    powerVector.record(demand.getDemand());
    powerStats.collect(demand.getDemand());


    double p = (double)rand() / RAND_MAX;
    double fMax=1000;
    double fMin=500;              //Vehicle kW hours
    double supplyPower= fMin + p * (fMax - fMin);

    double maxWind=15;
    double minWind=13;              //Vehicle kW hours
  //  double supplySpeed= minWind + p * (maxWind - minWind);
  //  double supplyWindPower=0.625*715.0*supplySpeed*supplySpeed*supplySpeed/1000;
    supply==2000;

    //costStats.collect(demand.getCost());
 //   costVector.record(demand.getCost());
 //   std::cout<<totalChargingLevel<< ":"<< simTime()<< std::endl;

    if (((tickTocks>=400))){
           //if (supply==500.0)
           //    supply=750.0;
           //else
        supply=500;

                    if (totalChargingLevel>supply)
                            {
         //               std::cout<<"Sending broadcast is"<<simTime()<<endl;

                                 sendMessage(true);
                            }

       }

    /*
    else if (((tickTocks<6000))&& (tickTocks>5000)){
        //if (supply==500.0)
        //    supply=750.0;
        //else
            //supply=750;
            supply=supplyWindPower;

                 if (totalChargingLevel>supply)
                         {
      //               std::cout<<"Sending broadcast is"<<simTime()<<endl;

                              sendMessage(true);
                         }

    }else if ( (tickTocks>=6000)&&((tickTocks % 500)==0) ){

         //supply=supplyPower;
        supply=supplyWindPower;
       // supply=750;
                        if (totalChargingLevel>supply)
                                {
             //               std::cout<<"Sending broadcast is"<<simTime()<<endl;

                                     sendMessage(true);
                                }


    }
    */

    else {
        if (totalChargingLevel>supply)
                                 {
   //         std::cout<<"Sending broadcast is"<<simTime()<<endl;

                                      sendMessage(true);
                                 }


    }


    /*
    else {
        supply=50.0;
                 if (totalChargingLevel>supply)
                         {
                              sendMessage(true);
                         }

    }

    if (tickTocks<100){
        supply=100.0;
        if (totalChargingLevel>supply)
        {
             sendMessage(true);
        }
    }else if ((tickTocks>=100) && (tickTocks<150) ){
        supply=50.0;
         if (totalChargingLevel>supply)
                 {
                      sendMessage(true);
                 }
     }else if ( (tickTocks>=150) && (tickTocks<200) ){
         supply=100.0;
         if (totalChargingLevel>supply)
                         {
                              sendMessage(true);
                         }

     }else if ( (tickTocks>=200) && (tickTocks<250) ){
         supply=50.0;
         if (totalChargingLevel>supply)
                         {
                              sendMessage(true);
                         }
     }else if ( (tickTocks>=250)){
         supply=100.0;
         if (totalChargingLevel>supply)
                         {
                              sendMessage(true);
                         }
     }
     */

       //      sendMessage(false);
    supplyVector.record(supply);
    //utilisationVector.record(demand.getDemand()/supply);
   //if (supply!=0)
    powerStorage.record(demand.getStorage());
    utilisation=demand.getDemand()/supply;
    if ( (demand.getDemand()!=0) && (tickTocks>=4000) ){
        //utilisationStats.collect(demand.getDemand()/supply);
    }
}

void TraCIDemoRSU11p::onData(WaveShortMessage* wsm) {
    bool foundId=false;
    findHost()->getDisplayString().updateWith("r=16,green");

    //Check if there is an entry regarding on road vehicle data
 //   for(std::vector<vehicleData>::iterator it = vehicledata.begin(); it != vehicledata.end(); ++it)
 //   {
//            if ((*it).vehicleID==wsm->getVehicleId())
//            {
 //               totalChargingLevel+=wsm->getCurrentChargingRate()-(*it).chargingPower;
 //               (*it).chargingPower=wsm->getCurrentChargingRate();
 //               foundId=true;
 //           }

 //   }
 //   vehicleData mondata(wsm->getVehicleId(), wsm->getCurrentChargingRate());
//    if (!foundId){
 //       vehicledata.push_back(mondata);
      //  totalChargingLevel+=wsm->getCurrentChargingRate();
  //  }

    //annotations->scheduleErase(1, annotations->drawLine(wsm->getSenderPos(), mobi->getCurrentPosition(), "blue"));
    //std::cout<<totalChargingLevel<< ":"<< simTime()<< std::endl;


    /*
    //Update total charging power if you find it in the list
    if ( std::find_if(vehicleData.begin(), vehicleData.end(), item) != vehicleData.end() )
        totalChargingLevel+=wsm->currentChargingRate-previousRAte;
    else
        totalChargingLevel+=wsm->currentChargingRate;;
    */
    //std::cout<<totalChargingLevel<<endl;

//    if (totalChargingLevel>1000.0)
 //   {
 //            sendMessage(true);
 //    }
 //    else
 //          sendMessage(false);

}

void TraCIDemoRSU11p::sendMessage(bool congestion) {
	sentMessage = true;
	t_channel channel = dataOnSch ? type_SCH : type_CCH;
	WaveShortMessage* wsm = prepareWSM("data", dataLengthBits, channel, dataPriority, -1,2);
	wsm->setCongestionSignal(congestion);
	sendWSM(wsm);
}
void TraCIDemoRSU11p::sendWSM(WaveShortMessage* wsm) {
	sendDelayedDown(wsm,individualOffset);
}


