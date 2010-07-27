#!/bin/bash

./average.sh

pause "Now kill-rectangle and yank it"

gnuplot gnu.plot
epstopdf test.pdf
