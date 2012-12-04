#!/bin/bash

a="smi"
b="txt"
c="./learning_set"

cp ./training_set/*.smi ./

for filename in *.$a
do
    echo "Parsing $filename"
    ./extractor $filename $c/${filename%$a}$b
done

rm *.smi

exit 0    