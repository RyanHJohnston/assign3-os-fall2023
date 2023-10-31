#!/bin/bash

make all

for i in {1..4};
do
    make fifo >> output.txt
    make sjf >> output.txt
    make pr >> output.txt
done

make clean
