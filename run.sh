#!/bin/bash

make all

for i in {1..4};
do
    make fifo
    make sjf
    make pr
done

printf "\nThank you for using this program!\n"
printf "Email me at ryan.johnston@utsa.edu for any questions or concerns.\n"
