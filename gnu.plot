set terminal postscript eps color enhanced
set output 'test.eps'
set xlabel 'Grain'
set ylabel 'Speed up'
set title 'Speed UP on Sequential Blur'
set grid
plot 'blur_dfg_xkaapi.data' using 1:($3/$2) title 'DFG version' with linespoints, \
     'blur_adapt_xkaapi.data' using 1:($3/$2) title 'Adapt version' with linespoints, \
     'blur_adapt_xkaapi_half.data' using 1:($3/$2) title 'Adapt version (half steal)' with linespoints
