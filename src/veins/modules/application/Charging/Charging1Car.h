//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#ifndef CHARGING1CAR_H
#define CHARGING1CAR_H

#include "veins/modules/application/ieee80211p/BaseWaveApplLayer.h"
#include "veins/modules/mobility/traci/TraCIMobility.h"
#include "veins/modules/mobility/traci/TraCICommandInterface.h"

using Veins::TraCIMobility;
using Veins::TraCICommandInterface;
using Veins::AnnotationManager;

class Charging1Car : public BaseWaveApplLayer {
    public:
        virtual void initialize(int stage);
//        virtual void receiveSignal(cComponent* source, simsignal_t signalID, cObject* obj, cObject* details);
    protected:
        TraCIMobility* mobility;
        TraCICommandInterface* traci;
        TraCICommandInterface::Vehicle* traciVehicle;
        AnnotationManager* annotations;
    protected:
        virtual void onBeacon(WaveShortMessage* wsm);
        virtual void onData(WaveShortMessage* wsm);
        void sendMessage(std::string blockedRoadId);
//        virtual void handlePositionUpdate(cObject* obj);
//        virtual void handleParkingUpdate(cObject* obj);
        virtual void sendWSM(WaveShortMessage* wsm);
    protected:
        virtual void onTimer(cMessage* msg);
};

#endif /* SRC_VEINS_MODULES_APPLICATION_CHARGING_CHARGING1CAR_H_ */
