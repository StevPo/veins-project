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

#include "veins/modules/application/Charging/Charging1Car.h"

using Veins::TraCIMobilityAccess;
using Veins::AnnotationManagerAccess;

Define_Module(Charging1Car);

void Charging1Car::initialize(int stage) {
    BaseWaveApplLayer::initialize(stage);
    if (stage == 0) {
        mobility = TraCIMobilityAccess().get(getParentModule());
        traci = mobility->getCommandInterface();
        traciVehicle = mobility->getVehicleCommandInterface();
        annotations = AnnotationManagerAccess().getIfExists();
        ASSERT(annotations);
    }
}

void Charging1Car::onTimer(cMessage* msg) {
 //   DBG << "TimerCar! Name: " << msg->getName() << endl;
}

void Charging1Car::onBeacon(WaveShortMessage* wsm) {
}

void Charging1Car::onData(WaveShortMessage* wsm) {
}

void Charging1Car::sendMessage(std::string blockedRoadId) {
}

void Charging1Car::sendWSM(WaveShortMessage* wsm) {
}


