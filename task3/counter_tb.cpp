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

    vbdSetMode(1);
    // initialise initial signal levels for simulation inputs
    top->clk=1;
    top->rst=1;
    top->v=4;
    top->ld=0;

    // run simulation for many clock cycles - this is where simulation happens
    for (i = 0; i < 2000; i++) {
    top->rst = (i < 2) || (i == 17);
    top->ld  = vbdFlag();

        for (clk = 0; clk < 2; clk++) {
            tfp->dump(2*i + clk);
            top->clk = !top->clk;
            top->eval();
        }
        // ++++ Send count value to Vbuddy
        vbdHex(4, (int(top->count) >> 16) & 0xF);
		vbdHex(3, (int(top->count) >> 8) & 0xF);
		vbdHex(2, (int(top->count) >> 4) & 0xF);
		vbdHex(1, int(top->count) & 0xF);
        vbdCycle(i+1);
        // ---- End of Vbuddy output section

        if(Verilated::gotFinish()) exit(0);
    }
    vbdClose();
    tfp->close();
    exit(0);
}