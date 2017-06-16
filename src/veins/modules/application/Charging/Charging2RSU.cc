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

#include "veins/modules/application/Charging/Charging2RSU.h"

using Veins::AnnotationManagerAccess;
using Veins::TraCIScenarioManagerAccess;
using namespace std;

double sumDemand = 0;

Define_Module(Charging2RSU);

void Charging2RSU::initialize(int stage) {
    BaseWaveApplLayer::initialize(stage);
    if (stage == 0) {
        mobi = dynamic_cast<BaseMobility*> (getParentModule()->getSubmodule("mobility"));
        ASSERT(mobi);
        annotations = AnnotationManagerAccess().getIfExists();
        ASSERT(annotations);
        traci = TraCIScenarioManagerAccess().get();
        ASSERT(traci);
        sentMessage = false;

        SumD.setName("Demand (100kW)");
        supply = getParentModule()->par("supply").doubleValue();
        alpha = getParentModule()->par("alpha_init").doubleValue();
        kappa = getParentModule()->par("kappa").doubleValue();
        minPrice = getParentModule()->par("minPrice").doubleValue();
        price = minPrice;
        Price.setName("Price (cents/kWh)");
    }
}

void Charging2RSU::onTimer(cMessage* msg) {

    SumD.record(sumDemand);

    /* Evaluate price p[i] (cents/kWh) */
    price = alpha*(pow((sumDemand/supply),kappa));
    if (price < minPrice) {
        price = minPrice;
    }
    Price.record(price);

    //EV << "SumDemand: " << sumDemand << endl;
    //EV << "Price: " << price << endl;

    sendMessage(price);

}

void Charging2RSU::onBeacon(WaveShortMessage* wsm) {
}

void Charging2RSU::onData(WaveShortMessage* wsm) {

}

void Charging2RSU::sendMessage(double price) {
    sentMessage = true;
    t_channel channel = dataOnSch ? type_SCH : type_CCH;
    WaveShortMessage* wsm = prepareWSM("data", dataLengthBits, channel, dataPriority, -1,2);
    wsm->setPrice(price);
    sendWSM(wsm);
}

void Charging2RSU::sendWSM(WaveShortMessage* wsm) {
    sendDelayedDown(wsm,individualOffset);
}
