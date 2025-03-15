#ifndef __WRRPROJECT_SERVER_H_
#define __WRRPROJECT_SERVER_H_

#include <omnetpp.h>
#include "WrrPacket_m.h"

using namespace omnetpp;

class Server : public cSimpleModule {
  private:
    long recvCount;
    simsignal_t rcvdSignal;
    simsignal_t latencySignal;

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

#endif
