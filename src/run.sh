#!/bin/bash
make
echo "Start testing"
echo "GShare with 13 bits global history:"
for file in $(ls ../traces)
do
bunzip2 -kc ../traces/$file |./predictor --gshare:13 | grep "Misprediction Rate:"
done
echo "Tournament with 9 bits global history, 10 bits local history, 10 PC bits:"
for file in $(ls ../traces)
do
bunzip2 -kc ../traces/$file |./predictor --tournament:9:10:10 | grep "Misprediction Rate:"
done
echo "Custom Predictor:"
for file in $(ls ../traces)
do
bunzip2 -kc ../traces/$file |./predictor --custom | grep "Misprediction Rate:"
done
make clean