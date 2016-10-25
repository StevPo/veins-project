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

#include "application/traci/TraCIDemo11p.h"
#include "PowerDemand.h"

using Veins::TraCIMobilityAccess;
using Veins::AnnotationManagerAccess;

const simsignalwrap_t TraCIDemo11p::parkingStateChangedSignal = simsignalwrap_t(TRACI_SIGNAL_PARKING_CHANGE_NAME);

Define_Module(TraCIDemo11p);

void TraCIDemo11p::initialize(int stage) {

    BaseWaveApplLayer::initialize(stage);
    srandom(time(NULL));
    id = rand();
    if (stage == 0) {
        traci = TraCIMobilityAccess().get(getParentModule());
        annotations = AnnotationManagerAccess().getIfExists();
        ASSERT(annotations);

        sentMessage = false;
        lastDroveAt = simTime();
        findHost()->subscribe(parkingStateChangedSignal, this);
        isParking = false;
        sendWhileParking = par("sendWhileParking").boolValue();
        probabilityVector.setName("Probability");
        desiredChargingRateVector.setName("DesiredChargingRate");
        costVector.setName("Cost vector");

    }
    //Initialise vehicle energy requirements
        //srandom(time(NULL));
        srandom(time(NULL));
        double p = (double)rand() / RAND_MAX;
        coinToss = (double)rand() / RAND_MAX;
        f=0.06;
        double fMax=72000000;
        double fMin=10000000;              //Vehicle kW hours
        requestedEnergy= fMin + p * (fMax - fMin);
        vehicleEnergy=0.0;
        oldPower=1;
        power=1;
        count=0;
        id=traci->getExternalId();
        remainingChargingTime=false;
       // std::cout<<"module appeared at"<< ":"<< simTime()<< std::endl;
        vehicledata=std::vector<vehicleData>();
        chargingData=std::vector<vehicleChargingData>();




}

void TraCIDemo11p::onBeacon(WaveShortMessage* wsm) {

}

double TraCIDemo11p::calculateDerivative(double avgPower) {
    double v1=0.5;
    double derivative= 3*v1*avgPower*avgPower - 0.0001/avgPower-0.0001/(maximumPower-avgPower);
    return derivative;
}

double TraCIDemo11p::calculateAverage(std::vector<vehicleChargingData> chargingData) {

    double delaySeconds=5.0;
    double chargingPowerSum=0.0;
    double chargingPowerSamples=0.0;
    double average=0.0;
    //Average
    for(std::vector<vehicleChargingData>::iterator it = chargingData.begin(); it != chargingData.end(); ++it)
       {
               if ((*it).timestamp<simTime()-delaySeconds)
               {

               }else {
                   //Update the charging ID.
                               chargingPowerSum+=(*it).chargingPower;
                               chargingPowerSamples++;

               }
       }
    average=chargingPowerSum/chargingPowerSamples;
    return average;
}

void TraCIDemo11p::onTimer(cMessage* msg)
{

    //Send message including the derivative of the cost function and
    //the vehicle ID.

    //Here calculate the derivative of remaining energy/power

    /*
    double derivative= - (vehicleEnergy/(power*power));
    double avg=calculateAverage(chargingData);
    double leastCostDerivative=calculateDerivative(avg);

    sendMessage(id,derivative);
*/

    //get the timestamp of the module appearing
    count++;
    //Aimd increase rate is set accprding to the per/sec rate
    //We have to find the granularity of the timestep and normalise
    //according to it
    double speed=traci->getSpeed();
    double time1=spacingLength/traci->getSpeed();
    //Ttoal charging power is equal to the overall value
    //divided by the number of vehicles in the charging lane

    //a=5;
     //     std::cout<<"time is "<<time1<<endl;
     //     std::cout<<"spacingLength is "<<spacingLength<<endl;
     //     std::cout<<"speed is "<<traci->getSpeed()<<endl;
     //     std::cout<<"timestamp is "<<simTime()<<endl;
     //     std::cout<<"vehicle is "<<traci->getExternalId()<<endl;


    a=10*time1;
    PowerDemand demand=PowerDemand();
    mtx.lock();
    double modulo= ceil(chargingPadLength/spacingLength)+1;

    demand.removeDemand(power);
    int mod=(int) modulo;
 //   std::cout<<"mod is "<<mod<<endl;

    if (vehicleEnergy<requestedEnergy)
    {
    //Modulate the count in order to take vehicle speed into consideration//
//        if ( (count % mod!=0))
//        {

            /*
            if (energyStorage>0)
            {

                power=oldPower;

                if (power>=maximumPower){
                    power=maximumPower;
                    oldPower=power;
                }
                else
                {
                    power+=a;
                    oldPower=power;
                }

                powerStorage=energyStorage/time1;
                energyStorage=0;
                storageVectorEnergy.record(energyStorage);
                //here the part with energy storage
                demand.addDemand(power);
                storageVector.record(powerStorage);
                powerVector.record(power-powerStorage);

            }

            else

            {

*/
                power=oldPower;
                if (power>=maximumPower){
                    power=maximumPower;
                    oldPower=power;
                }
                else
                {
                    power+=a;
                    oldPower=power;
                }

                demand.addDemand(power);
                storageVector.record(0);
                powerVector.record(power);

 //           }

 //      }

                /*
        else
        {
            power=0;

            //powerStorage=oldPower;
            //energyStorage+=powerStorage*time1;
            //storageVectorEnergy.record(energyStorage);
            //storageVector.record(-powerStorage);
            powerVector.record(power);
            //demand.addDemand(powerStorage);
                     if (powerStorage>maximumPower){
                         powerStorage=maximumPower;
                         oldPower=powerStorage;
                     }
           //          else
           //          {
           //              power+=a;
           //              oldPower=power;
           //          }


        }

        */
        //push  power and timestamp into the list
        vehicleChargingData data(power, simTime());
        chargingData.push_back(data);

    }

 //   else {

 //       power=0;

 //   }
    //vehicleEnergy+=power*time1;
    //demand.addCost(power*power*power*0.5);
    //energyVector.record(vehicleEnergy);
    //powerStats.collect(power);
    mtx.unlock();
}

void TraCIDemo11p::onData(WaveShortMessage* wsm) {
    bool foundId=false;
    double gamma=0.00011;


	//get simulation parameters and if there is a congestion signal
    //make the update on power
    //Check if there is an entry regarding on road vehicle data
    for(std::vector<vehicleData>::iterator it = vehicledata.begin(); it != vehicledata.end(); ++it)
    {
            if ((*it).vehicleID==wsm->getVehicleId())
            {
                //Update the charging ID.
                (*it).chargingPower=wsm->getCurrentChargingRate();
                //(*it).=wsm->getCurrentChargingRate();
                foundId=true;
            }

    }

    vehicleData mondata(wsm->getVehicleId(), wsm->getCurrentChargingRate());
    if (!foundId){
        vehicledata.push_back(mondata);
      //  totalChargingLevel+=wsm->getCurrentChargingRate();
    }

    PowerDemand demand=PowerDemand();
    findHost()->getDisplayString().updateWith("r=16,green");
	annotations->scheduleErase(1, annotations->drawLine(wsm->getSenderPos(), traci->getPositionAt(simTime()), "blue"));

	//if (traci->getRoadId()[0] != ':') traci->commandChangeRoute(wsm->getWsmData(), 9999);
	//if (!sentMessage) sendMessage(wsm->getWsmData());

	//make the update on power to be received when there is congestions
	if (wsm->getCongestionSignal()){

//	    std::cout<<"Transmission timestamp is"<<wsm->getCreationTime()<<endl;
 //       std::cout<<"Transmission timestamp is"<<simTime()<<endl;


	    mtx.lock();
	    double fnew;
	    if (vehicledata.size()!=0){
     //       double desiredRate=calculateDesiredChargingRate(oldPower);
     //       fnew=f-gamma*(desiredRate-oldPower);
       //     powerVector.record(oldPower);
        }
	    else
	        fnew=f;
	    //double fnew=1;
        coinToss = (double)rand() / RAND_MAX;
	    if (coinToss<f)
	    {
//	        std::cout<<"Initial power was "<<oldPower<<std::endl;
            //demand.removeDemand(power);
	        oldPower=oldPower*b2;
            //demand.addDemand(power);
//	        std::cout<<"Big Power reduction to "<<oldPower<<std::endl;
	    }
       else
	    {
           //demand.removeDemand(power);
    //       std::cout<<"Initial power was "<<oldPower<<std::endl;
           oldPower=oldPower*b1 ;
  //         std::cout<<"Power reduction to "<<oldPower<<std::endl;

           //demand.addDemand(power);
	    }

//	    f=fnew;
        probabilityVector.record(f);
        mtx.unlock();

	}
}



double TraCIDemo11p::calculateDesiredChargingRate(double power){

    //The desired charging rate
    double desiredChargingRate;
    double gamma=0.01;
    double derivativeDifferential=0.0;

    double avg=calculateAverage(chargingData);
    double derivative=calculateDerivative(avg);
    //double derivative= calculateDerivative(power);

    for(std::vector<vehicleData>::iterator it = vehicledata.begin(); it != vehicledata.end(); ++it)

    {
        derivativeDifferential+= (*it).chargingPower-derivative;

    }

    desiredChargingRate= power + (gamma/  vehicledata.size()) * derivativeDifferential;

   // averagePowerVector.record(avg);
  //  derivativePowerVector.record(derivative);
    powerVector.record(power);
    desiredChargingRateVector.record(desiredChargingRate);


    return desiredChargingRate;
}

void TraCIDemo11p::sendMessage(std::string vehicleId, double chargingPower) {
    sentMessage = true;

    t_channel channel = dataOnSch ? type_SCH : type_CCH;
    WaveShortMessage* wsm = prepareWSM("data", dataLengthBits, channel, dataPriority, -1,2);
    wsm->setWsmData("");
    wsm->setCongestionSignal(false);
    wsm->setCurrentChargingRate(chargingPower);
    wsm->setVehicleId(vehicleId.c_str());
    sendWSM(wsm);
}


void TraCIDemo11p::receiveSignal(cComponent* source, simsignal_t signalID, cObject* obj) {
	Enter_Method_Silent();
	if (signalID == mobilityStateChangedSignal) {
		handlePositionUpdate(obj);
	}
	else if (signalID == parkingStateChangedSignal) {
		handleParkingUpdate(obj);
	}
}
void TraCIDemo11p::handleParkingUpdate(cObject* obj) {

}
void TraCIDemo11p::handlePositionUpdate(cObject* obj) {

    //Here send the derivative of

    /*
    BaseWaveApplLayer::handlePositionUpdate(obj);
	        //the current one and subtract the energy required to charge from the overall charging energy
	        simtime_t previousInstance=currentInstance;
	        currentInstance=simTime();
	        PowerDemand demand=PowerDemand();
	        //int trade=demand.area();
	        simtime_t deltaDuration=currentInstance-previousInstance;
	        Coord chargingLaneEndPosition(2432,2107);
	        //if (traci->getCurrentPosition().y > 1750){
	        if (!remainingChargingTime){
	            double remainingTime=traci->getCurrentPosition().distance(chargingLaneEndPosition) / traci->getSpeed();
	            remainingChargingTime=true;
	            minimalChargingRate=(vehicleEnergy/20);
	            a=1;
	        }

	        //}
	        //std::cout<<"Delta duration is"<<deltaDuration.dbl()<<std::endl;
	        //vehicleEnergy = vehicleEnergy-((deltaDuration.dbl()*power)/3600.0);
	        //if (vehicleEnergy<0)
	        //        vehicleEnergy=0;

	        double initialPower=power;  BaseWaveApplLayer::handlePositionUpdate(obj);
            //the current one and subtract the energy required to charge from the overall charging energy
            simtime_t previousInstance=currentInstance;
            currentInstance=simTime();
            PowerDemand demand=PowerDemand();
            //int trade=demand.area();
            simtime_t deltaDuration=currentInstance-previousInstance;
            Coord chargingLaneEndPosition(2432,2107);
            //if (traci->getCurrentPosition().y > 1750){
            if (!remainingChargingTime){
                double remainingTime=traci->getCurrentPosition().distance(chargingLaneEndPosition) / traci->getSpeed();
                remainingChargingTime=true;
                minimalChargingRate=(vehicleEnergy/20);
                a=1;
            }

            //}
            //std::cout<<"Delta duration is"<<deltaDuration.dbl()<<std::endl;
            //vehicleEnergy = vehicleEnergy-((deltaDuration.dbl()*power)/3600.0);
            //if (vehicleEnergy<0)
            //        vehicleEnergy=0;

            double initialPower=power;
            demand.removeDemand(power);
                if (vehicleEnergy>0)
                {
                    if (power>maximumPower)
                        power=maximumPower;
                    else
                        power+=a;
                    std::cout<<"Added the following amount of power"<<power-initialPower<<std::endl;
                    demand.addDemand(power);
                    std::cout<<"Total power is "<<demand.getDemand()<<std::endl;

                }
                else
                {

                    power=0;
                }
            powerVector.record(demand.getDemand());
            powerStats.collect(demand.getDemand());
	        demand.removeDemand(power);
	            if (vehicleEnergy>0)
	            {
	                if (power>maximumPower)
	                    power=maximumPower;
	                else
	                    power+=a;
	                std::cout<<"Added the following amount of power"<<power-initialPower<<std::endl;
	                demand.addDemand(power);
	                std::cout<<"Total power is "<<demand.getDemand()<<std::endl;

	            }
	            else
	            {

	                power=0;
	            }
	        powerVector.record(demand.getDemand());
	        powerStats.collect(demand.getDemand());

	        //sendMessage(id,power);
*/

}
void TraCIDemo11p::sendWSM(WaveShortMessage* wsm) {
	if (isParking && !sendWhileParking) return;
	sendDelayedDown(wsm,individualOffset);
}
