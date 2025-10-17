// Mandatory header files
#include "Vcounter.h"
#include "verilated.h"
#include "verilated_vcd_c.h"

int main(int argc, char **argv, char **env){ 
    int i; // i counts the no. clock cycles to simulate
    int clk; // clk is the module clock signal

    Verilated::commandArgs(argc, argv);
    // initialise top level verilog instance
    Vcounter* top = new Vcounter; // This is the DUT
    // initialise trace dump
    Verilated::traceEverOn(true); // turn on signal tracing
    VerilatedVcdC* tfp = new VerilatedVcdC;
    top->trace (tfp, 99); 
    tfp->open ("counter.vcd"); // tell verilog to dump the waveform data into

    // initialise initial signal levels for simulation inputs
    // top is a top-level entity, only top-level signals are visible - why and what is top level signal
    top->clk=1;
    top->rst=1;
    top->en=0;

    // run simulation for many clock cycles - this is where simulation happens
    for (i = 0; i < 300; i++) {
        top->rst = (i < 2) || (i == 18);
        top->en  = ((i > 4) && (i < 14)) || (i > 15);
        // expect: after output 0A, counter will reset
        // actual: after output 0B, counter output 0B then get reset
        // why: reset signal not yet settled, will show up in next half cycle (asynch reset)
        for (clk = 0; clk < 2; clk++) {
            tfp->dump(2*i + clk);
            top->clk = !top->clk;
            top->eval();
        }
        if(Verilated::gotFinish()) exit(0);
    }
    tfp->close();
    exit(0);
}