#!/bin/bash
clear
make
echo "Start testing"

for file in $(ls ../traces)
do
echo "----------------------------------Testing on $file -------------------------------------"
echo "*GShare 13 bits global history:*************"
bunzip2 -kc ../traces/$file |./predictor --gshare:13 #| grep "Misprediction Rate:"
echo "********Tournament :9 bits global history, 10 bits local history, 10 PC bits*********"
bunzip2 -kc ../traces/$file |./predictor --tournament:9:10:10 #| grep "Misprediction Rate:"
echo "************Custom Predictor*******************"
bunzip2 -kc ../traces/$file |./predictor --custom #| grep "Misprediction Rate:"
done
make clean