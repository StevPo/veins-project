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
#include "veins/modules/application/Charging/Charging2RSU.h"

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
        timestampEnergy = 0;
        E.setName("timestampEnergy (kWh)");
        carEnergy = 0;
        demand = 0;
        BatterySize = getParentModule()->par("BatterySize").doubleValue();
        SoC = getParentModule()->par("initialSoC").doubleValue();
        state.setName("SoC");
        cost = 0;
        C.setName("Cost (cents)");
        sumCost = 0;
        maxChargingRate = getParentModule()->par("maxPower").doubleValue();
        a = getParentModule()->par("a").doubleValue();
        w = getParentModule()->par("w").doubleValue();
        g = getParentModule()->par("g").doubleValue();
        Dem.setName("CarDemand (100kW)");
        oldDemand = 0;
    }
}

void Charging2Car::onBeacon(WaveShortMessage* wsm) {
}

void Charging2Car::onData(WaveShortMessage* wsm) {

    /* Energy gained (kWh) */
    timestampEnergy = demand*100*getModuleByPath("rsu[0]")->par("RSUTimer").doubleValue()/3600;
    E.record(timestampEnergy);
    carEnergy += timestampEnergy;

    /* State of Charge */
    SoC += a*timestampEnergy/BatterySize;
    state.record(SoC);

    /* Car Demand x[i] (100kW) */
    if (SoC < 1) {
        demand += g*(w-demand*wsm->getPrice());
    }
    else {
        demand = 0;
    }
    Dem.record(demand);

    /* Cost of charging during i interval C[i] (cents) */
    cost = timestampEnergy * wsm->getPrice();
    C.record(cost);
    sumCost += cost;

    /* Add to sumDemand x (100kWh) */
    m.lock();
    sumDemand += demand - oldDemand;
    EV << "Demand: " << sumDemand << endl;
    m.unlock();

    oldDemand = demand;
}

void Charging2Car::finish() {

    demand = 0;
    Dem.record(demand);

    m.lock();
    sumDemand += demand - oldDemand;
    EV << "Demand: " << sumDemand << endl;
    m.unlock();

    recordScalar("Car charging cost", sumCost);
    recordScalar("Car charged energy", carEnergy);

}

void Charging2Car::sendMessage(std::string blockedRoadId) {
}

void Charging2Car::sendWSM(WaveShortMessage* wsm) {
}


