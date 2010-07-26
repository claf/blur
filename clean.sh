#!/bin/bash

for dir in `find . -type d -name "blur*"`; do
  cd $dir;
  make clean && echo -n "." || echo -e "$dir : \tNok";
  cd ..; 
done
cd libppm;
make clean && echo -n "." || echo -e "libppm : \tNok";
echo "";
cd ..;
rm -fr bin/ data/ 
