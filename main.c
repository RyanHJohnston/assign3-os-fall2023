#include "lib.h"
#include <stdio.h>
#include <stdlib.h>

/* -------- DIRECTIONS ----------------*/
/*
 * input: prog -alg [FIFO|SJF|PR|RR] [-quantum [integer (ms)]] -input [file name]
 * output:   Input File Name              : input file name

     CPU Scheduling Alg           : FIFO|SJF|PR|RR (quantum)

     CPU utilization              : ....

     Throughput                   : ....

     Avg. Turnaround time         : ....

     Avg. Waiting time in R queue : ....
*/



int main(int argc, char *argv[]) {
    
    if (is_valid(argc, argv) == 1) {
        printf("failure\n");
    } else {
        printf("success\n");
    }

    
    return 0;
}
