#!/bin/sh

for dir in .
do

for file in $dir/*.tga
do
echo Converting $file
len=$(expr length $file - 4)
file=$(expr substr $file 1 $len)
convert -verbose $file.tga $file.eps
convert -verbose $file.tga $file.png
echo
#rm -f $file.tga
#ini=$(expr length $dir + 2)
#onlyf=$(expr substr $file $ini $len)
done  #file

done  #dir
