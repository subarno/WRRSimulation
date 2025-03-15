#include "Switch.h"
#include "omnetpp/cstringtokenizer.h"

Define_Module(Switch);

Switch::Switch() {
    endTransmissionEvent = nullptr;
}

Switch::~Switch() {
    cancelAndDelete(endTransmissionEvent);
}

void Switch::initialize() {
    endTransmissionEvent = new cMessage("endTxEvent");

    std::string wStr = par("weights").stringValue();
    cStringTokenizer tokenizer(wStr.c_str());
    for (int i = 0; i < 4; i++) {
        std::string token = tokenizer.nextToken();
        weights[i] = std::stoi(token);
    }

    for(int i=0; i<4; i++) {
        packetQueues[i].setName(("queue"+std::to_string(i)).c_str());
    }

    linkRate = gate("out")->getTransmissionChannel()->par("datarate").doubleValue();

    queueLengthSignal = registerSignal("queueLengthSignal");
    dropSignal = registerSignal("dropSignal");
    wrrSelectionSignal = registerSignal("wrrSelectionSignal");

    currentQueue = 0;
    roundCounter = 0;
}

void Switch::handleMessage(cMessage *msg) {
    if (msg == endTransmissionEvent) {

        if (!allQueuesEmpty()) {

            int selectedQueue = getNextWRRQueue();

            if(!packetQueues[selectedQueue].isEmpty()){
                cPacket *pkt = (cPacket *)packetQueues[selectedQueue].pop();

                EV << "Sending Packet from Queue[" << selectedQueue << "], Queue Size before: " << packetQueues[selectedQueue].getLength()+1 << endl;

                send(pkt,"out");

                EV << "Packet sent. Queue[" << selectedQueue << "], Size after: " << packetQueues[selectedQueue].getLength() << endl;

                emit(queueLengthSignal, getTotalQueueLength());

                simtime_t txTime = pkt->getBitLength() / linkRate;
                scheduleAt(simTime() + txTime, endTransmissionEvent);
            }
            /*cPacket *pkt = (cPacket *)packetQueue.pop();
            send(pkt, "out");

            emit(queueLengthSignal, packetQueue.getLength());

            simtime_t txTime = pkt->getBitLength() / linkRate;
            scheduleAt(simTime() + txTime, endTransmissionEvent);*/
        }
    } else {
        //cPacket *pkt = check_and_cast<cPacket*>(msg);

        WrrPacket *wpkt = check_and_cast<WrrPacket*>(msg);
        int srcQueue = wpkt->getSourceId() % 4;

        EV << "Packet received from Client[" << srcQueue << "], Queue Size before: " << packetQueues[srcQueue].getLength() << endl;

        if (packetQueues[srcQueue].getLength() < queueCapacity) {
            packetQueues[srcQueue].insert(wpkt);
        } else {
            EV << "Queue for Client[" << srcQueue << "] is full, dropping packet !" <<endl;
            emit(dropSignal, 1);
            delete wpkt;
            return;
        }

        EV << "Packet added to Queue[" << srcQueue << "], Queue Size after: " << packetQueues[srcQueue].getLength() << endl;

        if (!endTransmissionEvent->isScheduled()) {
            scheduleAt(simTime(), endTransmissionEvent);
        }
    }
}

int Switch::getNextWRRQueue() {
    while(true) {
        if(!packetQueues[currentQueue].isEmpty()) {
            if(roundCounter < weights [currentQueue]) {
                roundCounter++;
                return currentQueue;
            }
            roundCounter = 0;
        }
        currentQueue = (currentQueue + 1) % 4;
    }
}

int Switch::getTotalQueueLength() {
    int total = 0;
    for (int i=0;i<4;i++) {
        total += packetQueues[i].getLength();
    }
    return total;
}

bool Switch::allQueuesEmpty() {
    for (int i=0;i<4;i++) {
        if(!packetQueues[i].isEmpty())
            return false;
    }
    return true;
}


        /*if(packetQueues[srcQueue].getLength() < queueCapacity){
            packetQueues[srcQueue].insert(wpkt);
        } else {
            EV << "Queue for Client[" << srcQueue << "] is full, dropping packet! " << endl;
        }
        if (endTransmissionEvent->isScheduled()) {
            EV << "Switch is busy, queuing packet from Client[" << srcQueue << "]" << endl;
            packetQueue.insert(pkt);
        } else {
            EV << "Switch is free, forwarding packet from Client[" << srcQueue << "]" << endl;
            send(pkt, "out");

            emit(wrrSelectionSignal, srcQueue);
            EV << "WRR selected queue: " << srcQueue << " based on weight: " << weights[srcQueue] << endl;

            simtime_t txTime = pkt->getBitLength() / linkRate;
            scheduleAt(simTime() + txTime, endTransmissionEvent);
        }

        emit(queueLengthSignal, packetQueue.getLength());
    }
}*/
