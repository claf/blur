#!/bin/bash

BINARIES="blur_seq blur_dfg_xkaapi blur_adapt_xkaapi blur_adapt_xkaapi_half"

for prog in $BINARIES; do
  echo "# $prog :" > $prog.data
  for i in `seq 20 20 200`; do
    echo -n "$i " >> $prog.data
    awk '/total/ {s=s+$2;d=d+1} END {print s/d}' "Time_"$prog"_"$i.txt >> $prog.data
  done
done
