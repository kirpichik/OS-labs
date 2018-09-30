#!/bin/sh

if [ -z $1 ] || [ -z $2 ]
then
  echo "Need arguments: <src_path> <dst_path>"
  exit 1
fi

if [ -e $2 ]
then
  echo "Destination path already exists."
  exit 1
fi

echo "Run cp -R..."
time cp -R $1 $2

echo "\nCleanup cp -R copy...\n"
rm -rf $2

echo "Run ./lab..."
time ./lab $1 $2

echo "\nCleanup ./lab copy..."
rm -rf $2

