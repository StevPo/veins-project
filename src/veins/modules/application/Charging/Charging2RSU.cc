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
#include "veins/modules/application/Charging/Charging2Car.h"

using Veins::AnnotationManagerAccess;
using Veins::TraCIScenarioManagerAccess;
using namespace Eigen;
using namespace std;

double sumDemand = 0;
int i = -14;

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

        cars = 0;
        max_cars = 0;
        w = 0;
        sum_w = 0;
        q = 0;
        sum_xeq = 0;
        w_factor = 1;
        g = 0;
        G.setName("g (rate of convergence)");
    }
}

void Charging2RSU::onTimer(cMessage* msg) {

    SumD.record(sumDemand);

    cars = traci->getManagedHosts().size();
    if (cars > max_cars) {
        max_cars = cars;
    }
EV << "cars: " << cars << endl;
    if (cars!=0) {

        w_vec.resize(cars);
        sum_w = 0;
        int j = 0;
        for (int i = 0; i < max_cars; i++){
EV << "i: " << i << endl;
            nodeName.str("");
            nodeName << "node[" << i << "].appl";
            node = nodeName.str().c_str();
            if (check_and_cast_nullable<Charging2Car*>(getSimulation()->getModuleByPath(node)) != nullptr) {
                w = check_and_cast_nullable<Charging2Car*>(getSimulation()->getModuleByPath(node))->w;
                w_vec[j] = w;
                sum_w += w;
EV << "node_name: " << nodeName.str() << endl;
EV << node << " w_vec[" << j << "] = " << w_vec[j] << endl;
                j++;
            }
        }
EV << "w_vec: " << w_vec << endl;

        alpha = getParentModule()->par("alpha_init").doubleValue();
        q = pow(alpha,1/(kappa+1))*pow(sum_w,kappa/(kappa+1));
        sum_xeq = sum_w/q;
        if (sum_xeq > supply) {
            w_factor = alpha * pow(supply,kappa+1) / sum_w;
            for (int i = 0; i < cars; i++){
                w_vec[i] *= w_factor;
            }
            sum_w = alpha * pow(supply,kappa+1);
            q = sum_w/supply;
            sum_xeq = supply;
        }
EV << "w_vec: " << w_vec << endl;
EV << " sum_w: " << sum_w << " xeq: " << sum_xeq << ", q: " << q << ", alpha: " << alpha << endl;

        xeq.resize(cars);
        xeq_sqrt.resize(cars);
        for (int i = 0; i < cars; i++){
            xeq[i] = w_vec[i]/q;
            xeq_sqrt[i] = pow(xeq[i],0.5);
        }

        dq = alpha*kappa*pow(sum_xeq,kappa-1);
//EV << "dq: " << dq << endl;
        Xd = xeq.asDiagonal();
//EV << "Xd: " << Xd << endl;
        Xd_sqrt = xeq_sqrt.asDiagonal();
//EV << "Xd_sqrt: " << Xd_sqrt << endl;
//EV << "Xdinverse: " << Xd.inverse() << endl;
        Wd = w_vec.asDiagonal();
//EV << "Wd: " << Wd << endl;
        ones = ones.Ones(cars, cars);
//EV << "ones: " << ones << endl;

        Z = (Wd*Xd.inverse()) + Xd_sqrt*ones*Xd_sqrt*dq;
//EV << "Z: " << Z << endl;

        EigenSolver<MatrixXd> Ze(Z);
        vec = abs(Ze.eigenvalues().real().array());
//EV << "Z eigenvalues: " << vec << endl;

        max = 0;
        for(int i=0;i<cars;i++)
        {
            if(vec[i]>max)
                max=vec[i];
        }

        g = 1/max;
        G.record(g);
EV << "g: " << g << endl;
        }

        /* Evaluate price p[i] (cents/kWh) */
        price = alpha*(pow(sumDemand,kappa));

        if (price < minPrice) {
            price = minPrice;
        }

        Price.record(price);

        /* Send new price to EVs */
        info.price = price;
        info.g = g;
        info.w_factor = w_factor;
        sendMessage(info);
    }

void Charging2RSU::onBeacon(WaveShortMessage* wsm) {
}

void Charging2RSU::onData(WaveShortMessage* wsm) {

}

void Charging2RSU::sendMessage(CarInfo info) {
    sentMessage = true;
    t_channel channel = dataOnSch ? type_SCH : type_CCH;
    WaveShortMessage* wsm = prepareWSM("data", dataLengthBits, channel, dataPriority, -1,2);
    wsm->setInfo(info);
    sendWSM(wsm);
}

void Charging2RSU::sendWSM(WaveShortMessage* wsm) {
    sendDelayedDown(wsm,individualOffset);
}
