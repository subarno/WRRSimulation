#include "Client.h"

Define_Module(Client);

Client::Client() {
    sendTimer = nullptr;
}

Client::~Client() {
    cancelAndDelete(sendTimer);
}

void Client::initialize() {
    clientId = par("clientId");
    iaTime = par("iaTime");
    packetLength = par("packetLength");

    sendTimer = new cMessage("sendTimer");
    scheduleAt(simTime() + exponential(iaTime), sendTimer);
}

void Client::handleMessage(cMessage *msg) {
    if (msg == sendTimer) {
        WrrPacket *pkt = new WrrPacket("Data");
        pkt->setSourceId(clientId);
        pkt->setByteLength(packetLength);

        pkt->setCreationTime(simTime());

        EV << "Client [" << clientId << "] sending packet of size" << packetLength << "bytes." << endl;

        cChannel *txChannel = gate("out")->getTransmissionChannel();
        if (!txChannel) {
            EV << "Error: No transmission channel found on output gate!\n";
            delete pkt;
            return;
        }

        double datarate = txChannel->par("datarate").doubleValue();
        if (datarate == 0) {
            EV << "Error: Transmission channel has no datarate set!\n";
            delete pkt;
            return;
        }

        send(pkt, "out");

        scheduleAt(simTime() + iaTime, sendTimer);
    }
}
