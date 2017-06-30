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

#ifndef CHARGING2RSU_H
#define CHARGING2RSU_H

#include "veins/modules/application/ieee80211p/BaseWaveApplLayer.h"
#include "veins/modules/world/annotations/AnnotationManager.h"
#include "veins/modules/mobility/traci/TraCIScenarioManager.h"
#include "veins/modules/messages/WaveShortMessage_m.h"
#include <math.h>


#include <iostream>
#include <string>
#include <sstream>
#include <Eigen/Dense>
#include <Eigen/Eigenvalues>


using Veins::AnnotationManager;
using Veins::TraCIScenarioManager;
using namespace Eigen;
using namespace std;

class Charging2RSU : public BaseWaveApplLayer {
    public:
        virtual void initialize(int stage);
    protected:
        TraCIScenarioManager* traci;
        AnnotationManager* annotations;
        BaseMobility* mobi;
        bool sentMessage;
    protected:
        virtual void onBeacon(WaveShortMessage* wsm);
        virtual void onData(WaveShortMessage* wsm);
        void sendMessage(CarInfo info);
        virtual void sendWSM(WaveShortMessage* wsm);
    protected:
        virtual void onTimer(cMessage* msg);

        /* sumDemand (extern) */
        cOutVector SumD;

        /* Network supply (max load) */
        bool restrSupply;
        double supply;

        /* Price parameters */
        double kappa;
        double minPrice;
        double price;
        cOutVector Price;
        double alpha;
        cOutVector Alpha;

        /* Get WTPi */
        int cars;
        const char *node;
        ostringstream nodeName;
        VectorXd w_vec;
        double w;
        double sum_w;

        /* Equilibrium values */
        double q;
        double sum_xeq;
        double w_factor;
        cOutVector wFactor;
        VectorXd xeq;
        VectorXd xeq_sqrt;

        /* variables for g parameter */
        double dq;
        MatrixXd Xd;
        MatrixXd Xd_sqrt;
        MatrixXd Wd;
        MatrixXd ones;
        MatrixXd Z;
        VectorXd vec;
        double max;
        double g;
        cOutVector G;

        /* message parameter */
        CarInfo info;
};

extern double sumDemand;

#endif /* SRC_VEINS_MODULES_APPLICATION_CHARGING_CHARGING2RSU_H_ */
