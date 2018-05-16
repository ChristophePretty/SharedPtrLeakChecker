#!/bin/bash

DEST="/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1/"
BAK="SharedPtrLeakChecker/original"
SRC="$PWD/SharedPtrLeakChecker"

FILES="memory vector map string functional set algorithm __string __tree"

for f in $FILES; do
    echo $f
    DEST_FILE="$DEST/$f"
    diff "$DEST_FILE" "$BAK/$f"
done

