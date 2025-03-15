##  Name of the Project : WRRSim

# Weighted Round Robin (WRR) Scheduler in OMNeT++

This project implements a **Weighted Round Robin (WRR)** packet scheduling mechanism in OMNeT++.  
It consists of:

- **4 Clients (`Client`)** generating traffic.
- **A Switch (`Switch`)** that implements WRR scheduling.
- **A Server (`Server`)** that receives and processes packets.

### 🔧 Features
- Clients generate **Poisson-distributed traffic**.
- Packets are scheduled based on **WRR weights**.
- The switch ensures **channel availability before transmission**.
- Transmission is queued properly, **avoiding "Channel is busy" errors**.

### 🏗 How to Use
1. Copy these files into a new OMNeT++ project (`WRRProject`).
2. Open the OMNeT++ IDE, **Build** the project.
3. **Run** the simulation with `omnetpp.ini`.
4. Adjust **weights, datarates, queue size** in `omnetpp.ini`.

### 📊 Observing Results
- The switch logs **queue lengths** and **packet drops**.
- The server records **received packets** and **latency**.
