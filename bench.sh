#!/bin/bash

echo "Starting Benchs";

BINARIES='blur_adapt_xkaapi blur_adapt_xkaapi_half'
#BINARIES='blur_dfg_xkaapi'
IMG=moon
ORIGIMG=/home/claferri/dev/blur/moon_blured.ppm
DIR=/home/claferri/dev/blur/data/
RUNS=`seq 1 50`
LD_LIBRARY_PATH=/home/claferri/opt/lib/

./clean.sh
./make.sh

mkdir -p $DIR
cd bin

# if [ `hostname` = "idkoiff" ]; then
#   echo "Numa Machine. Fixing sequential execution"
#   numa="numactl --physcpubind=0"
# else
#   numa=""
# fi

# # SEQ TIME :
# prog="blur_seq"
# echo "blur_seq"
# beg=$numa
# for execution in $RUNS
# do
#     echo "$execution for various block size :"
#     for block_size in `seq 20 10 200`
#     do
#         echo -n "$block_size "
#         LD_LIBRARY_PATH=$HOME/opt/lib/ $beg ./$prog ../$IMG.ppm $DIR$IMG"_"$prog"_"$execution"_"$block_size.ppm $block_size > tmp.txt
#         diff $DIR$IMG"_"$prog"_"$execution"_"$block_size.ppm $ORIGIMG && cat tmp.txt >> $DIR"Time_"$prog"_"$block_size.txt || cat tmp.txt >> error.log
#     done
#     echo ""
# done

for thread in 16 8 4 2 1
do
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

	if [ "$prog" = "blur_seq" ]; then
	    echo "blur_seq"
	    beg=$numa
	else
	    beg=""
	    if [ $thread -eq 16 ]; then
		KAAPI_CPUSET=`seq -s , 0 1 15`
		echo "CPUSET = all"
	    else
        endseq=$(( thread+thread-1 ))
		KAAPI_CPUSET=`seq -s , 0 2 $endseq`
		echo "CPUSET $KAAPI_CPUSET"
	    fi
	fi
	
	for execution in $RUNS
	do
	    echo "$execution for various block size :"
	    for block_size in `seq 20 10 200`
	    do
		echo -n "$block_size "
		LD_LIBRARY_PATH=$HOME/opt/lib/ KAAPI_CPUSET=$KAAPI_CPUSET ./$prog ../$IMG.ppm $DIR$IMG"_"$prog"_"$execution"_"$block_size.ppm $block_size $half_steal > tmp.txt
		diff $DIR$IMG"_"$prog"_"$execution"_"$block_size.ppm $ORIGIMG && cat tmp.txt >> $DIR"Time_"$prog"_"$block_size"_nbthread_"$thread.txt || cat tmp.txt >> error.log
	    done
	    echo ""
	done
    done
done

cd ../
