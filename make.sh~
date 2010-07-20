#!/bin/bash

for dir in 'blur_seq' 'blur_adapt_xkaapi' 'blur_dfg_xkaapi'; do
  cd $dir;
  make && echo -n "." || echo -e "$dir : \tNok";
  cd ..; 
done
