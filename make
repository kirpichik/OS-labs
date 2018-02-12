#!/bin/sh

# Makefile for systems with no make utility.
#
# Move this script to your project directory and
# define your compiler, flags, output file and 
# sources below.
# Then type ./make for build. Build results will be
# placed in a build/ folder.

OUTPUT_PATH="build/"

if [ ! -d $OUTPUT_PATH ]
then
  echo "Creating output dir..."
  mkdir $OUTPUT_PATH
fi

CC="gcc"
CC_FLAGS="-Wall"
LD_FLAGS="-lc"
OUTPUT="lab"
#SOURCE= !!! DEFINE YOUR SOURCE HERE !!!

if [ -z $SOURCE ]
then
  echo "Source files not defined. Edit this script and define it."
  exit 1
fi

EXECUTE="$CC $CCFLAGS -o $OUTPUT_PATH$OUTPUT $SOURCE $LD_FLAGS"

echo "Building sources with command: \"$EXECUTE\"..."
SECONDS=0

$EXECUTE

echo "Done in $SECONDS sec."

