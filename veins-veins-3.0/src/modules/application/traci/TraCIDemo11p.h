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

#ifndef TraCIDemo11p_H
#define TraCIDemo11p_H

#include "BaseWaveApplLayer.h"
#include "modules/mobility/traci/TraCIMobility.h"
#include "math.h"
#include <mutex>          // std::mutex
#include "modules/charging/vehicleData.h"
#include "modules/charging/vehicleChargingData.h"




using Veins::TraCIMobility;
using Veins::AnnotationManager;

/**
 * Small IVC Demo using 11p
 */
class TraCIDemo11p : public BaseWaveApplLayer {
	public:
		virtual void initialize(int stage);
		virtual void receiveSignal(cComponent* source, simsignal_t signalID, cObject* obj);
		double calculateDesiredChargingRate(double power);
	protected:
		TraCIMobility* traci;
		AnnotationManager* annotations;
		simtime_t lastDroveAt;
        std::vector<vehicleData> vehicledata;
        std::vector<vehicleChargingData> chargingData;

		bool sentMessage;
		bool isParking;
		bool sendWhileParking;
		static const simsignalwrap_t parkingStateChangedSignal;
		double a=10.0;
		double b1=0.97;
        double b2=0.97;
        double f;
        double coinToss;
        double power=1;
        double powerStorage=1;
        double energyStorage=0;
        double chargingPadLength=1.7;
        double spacingLength=0.02;
        double energy;
		double p=0.7;
		bool congestionSignal;
		bool chargingAuth;
	    std::string id;
	    double vehicleEnergy;
	    double requestedEnergy;
        bool remainingChargingTime;
        simtime_t currentInstance;
        double minimalChargingRate;
        double maximumPower=20.0;
        cLongHistogram powerStats;
        cOutVector powerVector;
        cOutVector energyVector;
        cOutVector probabilityVector;
        cOutVector averagePowerVector;
        cOutVector derivativePowerVector;
        cOutVector desiredChargingRateVector;
        cOutVector costVector;
        cOutVector storageVector;
        cOutVector storageVectorEnergy;




        int count;
        double oldPower;
        std::mutex mtx;           // mutex for critical section


	protected:
		virtual void onBeacon(WaveShortMessage* wsm);
		virtual void onData(WaveShortMessage* wsm);
        virtual void handlePositionUpdate(cObject* obj);
		virtual void handleParkingUpdate(cObject* obj);
		virtual void sendWSM(WaveShortMessage* wsm);
	    virtual void onTimer(cMessage* msg);
        void sendMessage(std::string vehicleId, double chargingPower);
        double calculateDerivative(double avgPower);
        double calculateAverage(std::vector<vehicleChargingData> chargingData);
};

#endif
