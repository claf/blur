#!/bin/sh

THREADS="1 2 4 8 16"

T="BenchTime_nbthread_"
for TH in ${THREADS} ; do
  PDFFILE="plot_speedup_${TH}.pdf"
  echo "Generating ${PDFFILE}"
  DATAFILE="${T}${TH}.data"
  EPSFILE="plot_speedup_${TH}.eps"

  echo "Warning : hard coded binaries order!!!"

  gnuplot << EOF
  set terminal postscript eps color enhanced
  set output '${EPSFILE}'
  set xlabel 'Grain (block size)'
  set ylabel 'Speedup'
  set title 'Speed Up for ${TH} threads'
  set grid

  plot '${DATAFILE}' using 1:(\$2/\$5):(\$2/\$6):(\$2/\$7) title 'DFG' with errorlines linewidth 3, \
       '${DATAFILE}' using 1:(\$2/\$8):(\$2/\$9):(\$2/\$10) title 'Adapt' with errorlines linewidth 3, \
       '${DATAFILE}' using 1:(\$2/\$11):(\$2/\$12):(\$2/\$13) title 'Adapt half' with errorlines linewidth 3

EOF
  epstopdf --nocompress --outfile="${PDFFILE}" "${EPSFILE}"
  rm ${EPSFILE}
  echo "Output file: ${PDFFILE}"
done
