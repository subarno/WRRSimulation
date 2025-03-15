#ifndef __WRRPROJECT_CLIENT_H_
#define __WRRPROJECT_CLIENT_H_

#include <omnetpp.h>
#include "WrrPacket_m.h"

using namespace omnetpp;

class Client : public cSimpleModule {
  private:
    int clientId;
    simtime_t iaTime;
    int packetLength;
    cMessage *sendTimer;

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;

  public:
    Client();
    virtual ~Client();
};

#endif
