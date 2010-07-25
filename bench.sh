#!/bin/bash

echo "Starting Benchs";

BINARIES='blur_seq blur_adapt_xkaapi blur_adapt_xkaapi_half blur_dfg_xkaapi'
IMG=moon
ORIGIMG=/home/claferri/dev/blur/moon_blured.ppm
DIR=/home/claferri/dev/blur/data/

LD_LIBRARY_PATH=/home/claferri/opt/lib/

./clean.sh
./make.sh

mkdir -p $DIR
cd bin

for prog in $BINARIES
do
    echo -n "Now benching $prog"

    if [ "$prog" = "blur_adapt_xkaapi_half" ]; then
	half_steal=1
	echo " with half_steal on" 
    else
	half_steal=0
	echo ""
    fi

    for execution in `seq 1 10`
    do
	echo "$execution for various block size :"
	for block_size in `seq 20 10 200`
	do
	    echo -n "$block_size "
	    ./$prog ../$IMG.ppm $DIR$IMG"_"$prog"_"$execution"_"$block_size.ppm $block_size $half_steal >> $DIR"Time_"$prog"_"$block_size.txt
	    diff $DIR$IMG"_"$prog"_"$execution"_"$block_size.ppm $ORIGIMG || echo "File $IMG"_"$prog"_"$execution"_"$block_size.ppm differ from original!"
	done
    done
done

cd ../

#awk sur les fichiers pour moyen dans fichier prog_grain
#??? variation du nombre de threads?
#courbes gnuplot par rapport à seq

