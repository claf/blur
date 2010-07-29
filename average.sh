#!/bin/bash

# Sequential time is treated differently :
BINSEQ="blur_seq"
BINARIES="blur_dfg_xkaapi blur_adapt_xkaapi blur_adapt_xkaapi_half"

FILENAME="BenchTime"

for thread in 1 2 4 8 16; do

    # file header :
    echo -ne "#grain\t$BINSEQ avg\tmin\tmax" > $FILENAME"_nbthread_"$thread".data" 
    for prog in $BINARIES;do
	echo -ne "\t$prog avg\tmin\tmax" >> $FILENAME"_nbthread_"$thread".data" 
    done
    # next line :
    echo "">> $FILENAME"_nbthread_"$thread".data" 

    for grain in `seq 20 20 200`; do
        # grain :
	echo -ne "$grain" >> $FILENAME"_nbthread_"$thread".data" 

	# sequential time :
	avg=`cat "Time_"$BINSEQ"_"$grain.txt | grep total | awk '{s=s+$2;d=d+1} END {print s/d}'`
	min=`cat "Time_"$BINSEQ"_"$grain.txt | grep total | awk '{print $2}' | sort -n | head -n 1`
	max=`cat "Time_"$BINSEQ"_"$grain.txt | grep total | awk '{print $2}' | sort -nr | head -n 1`
	echo -ne "\t$avg\t$min\t$max" >> $FILENAME"_nbthread_"$thread".data" 

	for prog in $BINARIES; do
	    # prog average :
	    avg=`cat "Time_"$prog"_"$grain"_nbthread_"$thread.txt | grep total | awk '{s=s+$2;d=d+1} END {print s/d}'`

	    # prog min :
	    min=`cat "Time_"$prog"_"$grain"_nbthread_"$thread.txt | grep total | awk '{print $2}' | sort -n | head -n 1`

	    # prog max :
	    max=`cat "Time_"$prog"_"$grain"_nbthread_"$thread.txt | grep total | awk '{print $2}' | sort -nr | head -n 1`

	    echo -ne "\t$avg\t$min\t$max" >> $FILENAME"_nbthread_"$thread".data" 
	done

	# next line :
	echo "" >> $FILENAME"_nbthread_"$thread".data" 

    done
done
