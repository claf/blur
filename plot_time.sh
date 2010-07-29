#!/bin/sh

THREADS="1 2 4 8 16"

T="BenchTime_nbthread_"
for TH in ${THREADS} ; do
  PDFFILE="plot_time_${TH}.pdf"
  echo "Generating ${PDFFILE}"
  DATAFILE="${T}${TH}.data"
  EPSFILE="plot_time_${TH}.eps"

  echo "Warning : hard coded binaries order!!!"

  gnuplot << EOF
  set terminal postscript eps color enhanced
  set output '${EPSFILE}'
  set xlabel 'Grain (block size)'
  set ylabel 'Time (s)'
  set title 'Time for ${TH} threads'
  set grid
  plot '${DATAFILE}' using 1:2:3:4 title 'Seq' with errorlines linewidth 3, \
       '${DATAFILE}' using 1:5:6:7 title 'DFG' with errorlines linewidth 3, \
       '${DATAFILE}' using 1:8:9:10 title 'Adapt' with errorlines linewidth 3, \
       '${DATAFILE}' using 1:11:12:13 title 'Adapt half' with errorlines linewidth 3

EOF
  epstopdf --nocompress --outfile="${PDFFILE}" "${EPSFILE}"
  rm ${EPSFILE}
  echo "Output file: ${PDFFILE}"
done
