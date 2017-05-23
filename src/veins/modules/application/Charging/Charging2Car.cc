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

#include "veins/modules/application/Charging/Charging2Car.h"

using Veins::TraCIMobilityAccess;
using Veins::AnnotationManagerAccess;
using namespace std;

Define_Module(Charging2Car);

void Charging2Car::initialize(int stage) {
    BaseWaveApplLayer::initialize(stage);
    if (stage == 0) {
        mobility = TraCIMobilityAccess().get(getParentModule());
        traci = mobility->getCommandInterface();
        traciVehicle = mobility->getVehicleCommandInterface();
        annotations = AnnotationManagerAccess().getIfExists();
        ASSERT(annotations);

        distance = 0;
        sumDistance = 0;
        chargingRatio = 1;
        ChR.setName("ChargingRatio");
        demand = 0;
        SoC = 0.5;
        a = getParentModule()->par("a").doubleValue();
        BatterySize = getParentModule()->par("BatterySize").doubleValue();
        w = getParentModule()->par("w").doubleValue();
        g = getParentModule()->par("g").doubleValue();
        Dem.setName("CarDemand");
        oldDemand = 0;
    }
}

void Charging2Car::onBeacon(WaveShortMessage* wsm) {
}

void Charging2Car::onData(WaveShortMessage* wsm) {


    //SoC += a*demand*getParentModule()->getSubmodule("RSU")->par("RSUTimer").doubleValue()/BatterySize;
    demand += g*(w-demand*wsm->getPrice());

    Dem.record(demand);

    m.lock();
    sumDemand += demand - oldDemand;
    EV << "Demand: " << sumDemand << endl;
    m.unlock();

    oldDemand = demand;

}

void Charging2Car::sendMessage(std::string blockedRoadId) {
}

void Charging2Car::sendWSM(WaveShortMessage* wsm) {
}


