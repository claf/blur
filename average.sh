#!/bin/bash

for i in `seq 20 20 200`; do echo -n "$i ";awk '/total/ {s=s+$2;d=d+1} END {print s/d}' Time_blur_seq_$i.txt; done
