#ifndef __WRRPROJECT_SWITCH_H_
#define __WRRPROJECT_SWITCH_H_

#include <omnetpp.h>
#include "WrrPacket_m.h"

using namespace omnetpp;

class Switch : public cSimpleModule {
  private:
    int weights[4];
    cQueue packetQueues [4];
    int currentQueue;
    int roundCounter;

    cMessage *endTransmissionEvent;
    double linkRate;
    int queueCapacity = 50;

    simsignal_t queueLengthSignal;
    simsignal_t dropSignal;
    simsignal_t wrrSelectionSignal;

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;

    int getNextWRRQueue();
    int getTotalQueueLength();
    bool allQueuesEmpty();

  public:
    Switch();
    virtual ~Switch();
};

#endif
