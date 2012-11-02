#!/bin/bash

## Permet d'avoir tous les vols dans un fichier
#cat *half* | \grep "vol --" | awk '{print $5 "\t" $3}' | sort -n > Vols.data

cd data
rm steal.tmp

#for j in `seq 1 1 5` `seq 5 10 200` `seq 200 200 2000`; do
for j in `seq 1 1 1900`; do
  echo -en $j "\t" >> steal.tmp
  cat *half* | grep "vol --" | awk '{if ( $5 == '"$j"' ){print $0}}' | wc -l >> steal.tmp
#  i=$j
done
cd ..

