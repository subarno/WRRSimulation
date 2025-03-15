

import omnetpp.*;
//import ned.*;

simple Client {
    parameters:
        int clientId;
        double iaTime @unit(s);
        int packetLength @unit(B);
    gates:
        output out;
}

simple Switch {
    parameters:
        string weights;
        int queueCapacity;
    gates:
        input in[4];
        output out;
}

simple Server {
    gates:
        input in;
}

channel ClientToSwitch extends ned.DatarateChannel {
    parameters:
        datarate = default(10Mbps);
}

channel SwitchToServer extends ned.DatarateChannel {
        parameters:
        datarate = default(10Mbps);
}

network WrrNetwork {
    submodules:
        client[4]: Client {
            parameters:
                clientId = index;
        }
        switch: Switch;
        server: Server;

    connections:
        client[0].out --> ClientToSwitch --> switch.in[0];
        client[1].out --> ClientToSwitch --> switch.in[1];
        client[2].out --> ClientToSwitch --> switch.in[2];
        client[3].out --> ClientToSwitch --> switch.in[3];
        switch.out --> SwitchToServer --> server.in;
}