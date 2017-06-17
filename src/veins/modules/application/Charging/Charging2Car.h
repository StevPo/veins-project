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

#ifndef CHARGING2CAR_H
#define CHARGING2CAR_H

#include "veins/modules/application/ieee80211p/BaseWaveApplLayer.h"
#include "veins/modules/mobility/traci/TraCIMobility.h"
#include "veins/modules/mobility/traci/TraCICommandInterface.h"

#include <iostream>
#include <mutex>

using Veins::TraCIMobility;
using Veins::TraCICommandInterface;
using Veins::AnnotationManager;
using namespace std;

class Charging2Car : public BaseWaveApplLayer {
    public:
        virtual void initialize(int stage);
        virtual void finish();
    protected:
        TraCIMobility* mobility;
        TraCICommandInterface* traci;
        TraCICommandInterface::Vehicle* traciVehicle;
        AnnotationManager* annotations;
    protected:
        virtual void onBeacon(WaveShortMessage* wsm);
        virtual void onData(WaveShortMessage* wsm);
        void sendMessage(std::string blockedRoadId);
        virtual void sendWSM(WaveShortMessage* wsm);
    protected:
        std::map<std::string, cModule*> totalVehicles;
        std::mutex m;

        /* Charging1 */
        double distance;
        double sumDistance;
        double chargingRatio;
        cOutVector ChR;

        const double ChargerLength = 1.7;
        const double ChargerGap = 0.3;

        /* Charging2 */

        /* Energy absorbed by car in a timestamp */
        double timestampEnergy;
        cOutVector E;

        /* Sum Energy absorbed by car */
        double carEnergy;

        /* Cost of energy absorbed during timestamp and whole */
        double cost;
        cOutVector C;
        double sumCost;

        /* CarDemand */
        double demand;
        cOutVector Dem;
        double oldDemand;

        /* Parameters */
        double BatterySize;
        double SoC;
        cOutVector state;
        double maxChargingRate;
        /* Charging efficiency */
        double a;
        /* Demand parameters */
        double w;
        double g;
};

#endif /* SRC_VEINS_MODULES_APPLICATION_CHARGING_CHARGING1CAR_H_ */
