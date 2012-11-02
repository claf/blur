#!/bin/sh

T="Vols.data"
  PDFFILE="plot_vol.pdf"
  echo "Generating ${PDFFILE}"
  DATAFILE="${T}"
  EPSFILE="plot_vol.eps"

  echo "Warning : hard coded binaries order!!!"

  gnuplot << EOF
  set terminal postscript eps color enhanced
  set output '${EPSFILE}'
  set xlabel 'Number of tasks replied'
  set ylabel 'Time'
  set title 'Steal'
  set grid

  plot '${DATAFILE}' title 'Tasks reply time'

EOF
  epstopdf --nocompress --outfile="${PDFFILE}" "${EPSFILE}"
  rm ${EPSFILE}
  echo "Output file: ${PDFFILE}"
