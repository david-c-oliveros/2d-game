#!/bin/bash
# Build script for engine

assembly="$1"

mkdir -p ../bin

# Get a list of all the .cpp files
cppFilenames=$(find game -type f -name "*.cpp" -print)
cppExternalFilenames="
external/SFMLAnimation/src/AnimationManager.cpp
"


compilerFlags="-g -fPIC -Wfatal-errors -w"

includeFlags="
-I/usr/include/SFML/
-Igame/include/
-Iexternal/
-Iexternal/glm/
-Iexternal/SFMLAnimation/src/
-Iexternal/SSBMTonberry/tileson/
"

linkerFlags="
-lsfml-graphics
-lsfml-window
-lsfml-system
-lsfml-audio
"

defines="-D_DEBUG"

echo "Building $assembly..."
echo g++ $cppFilenames $compilerFlags -o out/build/bin/$assembly $defines $includeFlags $linkerFlags
g++ $cppFilenames $cppExternalFilenames $compilerFlags -o out/build/bin/$assembly $defines $includeFlags $linkerFlags
