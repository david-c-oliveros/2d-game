#! /bin/sh

echo "opening file"
args=""
for file in "$@"; do
    args+=" src/$file.cpp include/$file.h"

    file_path="./include/$file.inl"
    if [[ -f "$file_path" ]]; then
        args+=" $file_path"
    fi
done

vim -p $args
