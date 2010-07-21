#!/bin/bash

for dir in `ls blur* -d`; do
  cd $dir;
  make clean && echo -n "." || echo -e "$dir : \tNok";
  cd ..; 
done
cd libppm;
make clean && echo -n "." || echo -e "libppm : \tNok";
echo "";
cd ..;