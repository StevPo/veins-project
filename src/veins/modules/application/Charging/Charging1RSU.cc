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

#include "veins/modules/application/Charging/Charging1RSU.h"

using Veins::AnnotationManagerAccess;
using Veins::TraCIScenarioManagerAccess;
using namespace std;

Define_Module(Charging1RSU);

void Charging1RSU::initialize(int stage) {
    BaseWaveApplLayer::initialize(stage);
    if (stage == 0) {
        mobi = dynamic_cast<BaseMobility*> (getParentModule()->getSubmodule("mobility"));
        ASSERT(mobi);
        annotations = AnnotationManagerAccess().getIfExists();
        ASSERT(annotations);
        traci = TraCIScenarioManagerAccess().get();
        ASSERT(traci);
        sentMessage = false;
    }
}

void Charging1RSU::onTimer(cMessage* msg) {

    if ( sumDemand <= supply ) {
        sendMessage(false);
    }
    else {
        sendMessage(true);
    }

}

void Charging1RSU::onBeacon(WaveShortMessage* wsm) {
}

void Charging1RSU::onData(WaveShortMessage* wsm) {

}

void Charging1RSU::sendMessage(bool congestion) {
    sentMessage = true;
    t_channel channel = dataOnSch ? type_SCH : type_CCH;
    WaveShortMessage* wsm = prepareWSM("data", dataLengthBits, channel, dataPriority, -1,2);
    wsm->setCongestion(congestion);
    sendWSM(wsm);
}

void Charging1RSU::sendWSM(WaveShortMessage* wsm) {
    sendDelayedDown(wsm,individualOffset);
}
