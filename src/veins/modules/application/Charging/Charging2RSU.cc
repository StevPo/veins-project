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

        /* SumDemand */
        SumD.setName("Demand (100kW)");

        /* Network supply (max load) */
        supply = getParentModule()->par("supply").doubleValue();

        /* Price parameters */
        kappa = getParentModule()->par("kappa").doubleValue();
        minPrice = getParentModule()->par("minPrice").doubleValue();
        price = minPrice;
        Price.setName("Price (cents/kWh)");
        alpha = getParentModule()->par("alpha_init").doubleValue();
        Alpha.setName("alpha price");
        /* alpha parameters */
        a_factor = 1;
        aFactor.setName("a_factor");
        counter = 0;
        flag = false;
        decrease = 0;
    }
}

void Charging2RSU::onTimer(cMessage* msg) {

    SumD.record(sumDemand);

    /* Evaluate a_facter & alpha due to network constrictions */
    if (sumDemand > 1.05*supply) {
        a_factor += 0.05;
        flag = true;
        counter = 0;
    }

    else if (sumDemand < 0.999*supply && flag && counter < 150) {
        counter++;
    }
    else if (sumDemand < 0.999*supply && flag) {
        a_factor -= 0.05;
        counter = 0;
    }

    aFactor.record(a_factor);

    alpha = a_factor*price/pow(supply,kappa);
    Alpha.record(alpha);

    /* Evaluate price p[i] (cents/kWh) */
    price = alpha*(pow(sumDemand,kappa));
    if (price < minPrice) {
        price = minPrice;
    }
    Price.record(price);

    /* Send new price to EVs */
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
