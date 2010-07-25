#!/bin/bash

mkdir -p bin

for dir in 'blur_seq' 'blur_adapt_xkaapi' 'blur_dfg_xkaapi'; do
  cd $dir;
  make && echo "" || echo -e "$dir : \tNok";
  cp blur ../bin/$dir;
  if [ "$dir" = "blur_adapt_xkaapi" ]; then
    cp blur "../bin/$dir"_half
  fi
  cd ..; 
done
