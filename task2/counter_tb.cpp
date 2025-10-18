// Mandatory header files
#include "Vcounter.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include "vbuddy.cpp"

int main(int argc, char **argv, char **env){
    int i;
    int clk;

    Verilated::commandArgs(argc, argv);
    // initialise top level verilog instance
    Vcounter* top = new Vcounter; // This is the DUT
    // initialise trace dump
    Verilated::traceEverOn(true); // turn on signal tracing
    VerilatedVcdC* tfp = new VerilatedVcdC;
    top->trace (tfp, 99); 
    tfp->open ("counter.vcd"); // tell verilog to dump the waveform data into

    // initialise Vbuddy connection - Port path is in vbuddy.cfg
    if (vbdOpen()!=1) return(-1);
    vbdHeader("Lab 1: Counter");

    // initialise initial signal levels for simulation inputs
    top->clk=1;
    top->rst=1;
    top->en=0;

    // run simulation for many clock cycles - this is where simulation happens
    for (i = 0; i < 2000; i++) {
    top->rst = (i < 2) || (i == 17);
    top->en  = vbdFlag();

        for (clk = 0; clk < 2; clk++) {
            tfp->dump(2*i + clk);
            top->clk = !top->clk;
            top->eval();
        }
        // ++++ Send count value to Vbuddy
        vbdPlot(int(top->count), 0, 255);
        vbdCycle(i+1);
        // ---- End of Vbuddy output section

        if(Verilated::gotFinish()) exit(0);
    }
    vbdClose();
    tfp->close();
    exit(0);
}