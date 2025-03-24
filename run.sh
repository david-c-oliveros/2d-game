#!/bin/bash

executable=""

if [ $# -ge 1 ]; then
    executable="$1"
else
executable="game"
fi

echo "running $executable"

pushd out/build/bin

./$executable

popd
