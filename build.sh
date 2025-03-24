#!/bin/bash
# Build script for building everything

args=""
if [ $# -ge 1 ]; then
    echo "args exist: $@"
    args="$1"
else
    args="game"
fi

echo "Building everything..."

#source compile.sh "$args"
./compile.sh "$args"

ERRORLEVEL=$?
if [ $ERRORLEVEL -ne 0 ]
then
    echo "Error:"$ERRORLEVEL && exit
fi

echo "All assemblies built successfully"
