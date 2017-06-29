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

        /* Charging1 */
        distance = 0;
        sumDistance = 0;
        chargingRatio = 1;
        ChR.setName("ChargingRatio");

        /* Charging2 */

        /* selfmessage to send WTP */
        sendWTP = getParentModule()->par("SendWTP").boolValue();

        /* Energy absorbed by car in a timestamp */
        new_time = simTime();
        old_time = 0;
        timestampEnergy = 0;
        E.setName("timestampEnergy (kWh)");

        /* Sum Energy absorbed by car */
        carEnergy = 0;

        /* Cost of energy absorbed during timestamp and whole */
        cost = 0;
        C.setName("Cost (cents)");
        sumCost = 0;

        /* Initial demand */
        demand = 0;
        Dem.setName("CarDemand (100kW)");
        oldDemand = 0;

        /* Parameters */
        BatterySize = getParentModule()->par("BatterySize").doubleValue();
        SoC = getParentModule()->par("initialSoC").doubleValue();
        state.setName("SoC");
        maxChargingRate = getParentModule()->par("maxChargingRate").doubleValue();
        minChargingRate = getParentModule()->par("minChargingRate").doubleValue();
        /* Charging efficiency */
        a = getParentModule()->par("a").doubleValue();
        /* Demand parameters */
        w = getParentModule()->par("w").doubleValue();//+i*0.1;
        i++;
        g = getParentModule()->par("g").doubleValue();
    }
}

void Charging2Car::onBeacon(WaveShortMessage* wsm) {
}

void Charging2Car::onTimer(cMessage* msg) {

    if (sendWTP) {
        EV << "sendmessageWTP" << endl;
        sendMessage(w);
    }
}

void Charging2Car::onData(WaveShortMessage* wsm) {

    info = wsm->getInfo();
    /* Energy gained (kWh) */
    old_time = new_time;
    new_time = simTime();
    timestamp = new_time - old_time;
    timestampEnergy = demand*100*timestamp.dbl()/3600;
    E.record(timestampEnergy);
    carEnergy += timestampEnergy;

    /* Cost of charging during i interval C[i] (cents) */
    cost = timestampEnergy * info.price;
    C.record(cost);
    sumCost += cost;

    /* State of Charge */
    SoC += a*timestampEnergy/BatterySize;
    state.record(SoC);

    /* Car Demand x[i] (100kW) */
    if (SoC < 1) {
        demand += info.g*(w*info.w_factor-demand*info.price);
        if (demand < minChargingRate) {
            demand = minChargingRate;
        }
    }
    else {
        demand = 0;
    }

    if (demand > maxChargingRate) {
        demand = maxChargingRate;
    }

    Dem.record(demand);

    /* Add to sumDemand x (100kWh) */
    m.lock();
    sumDemand += demand - oldDemand;
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

    recordScalar("Car charging cost (cents)", sumCost);
    recordScalar("Car charged energy (kWh)", carEnergy);

}

void Charging2Car::sendMessage(double wtp) {
    t_channel channel = dataOnSch ? type_SCH : type_CCH;
    WaveShortMessage* wsm = prepareWSM("data", dataLengthBits, channel, dataPriority, -1,2);
    wsm->setWtp(w);
    sendWSM(wsm);
}

void Charging2Car::sendWSM(WaveShortMessage* wsm) {
    sendDelayedDown(wsm,individualOffset);
}


