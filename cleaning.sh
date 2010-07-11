#!/bin/bash

for dir in `ls blur* -d`; do
  cd $dir;
  make clean && echo "$dir : Ok" || echo "$dir : Nok";
  cd ..; 
done
