#include "Server.h"

Define_Module(Server);

void Server::initialize() {
    recvCount = 0;
    rcvdSignal = registerSignal("rcvdSignal");
    latencySignal = registerSignal("latencySignal");
}

void Server::handleMessage(cMessage *msg) {
    WrrPacket *pkt = check_and_cast<WrrPacket*>(msg);
    recvCount++;

    simtime_t latency = simTime() - pkt->getCreationTime();

    EV << "Packet received from Client [" << pkt->getSourceId() << "]. Latency :" << latency << "s" << endl;
    //EV << "Packet received at Server. Latency : " << latency << "s" <<endl;

    emit(rcvdSignal, recvCount);
    emit(latencySignal, latency);

    recordScalar("PacketLatency", latency.dbl());

    delete pkt;
}
