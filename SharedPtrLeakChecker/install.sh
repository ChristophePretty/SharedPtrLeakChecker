#!/bin/bash

DEST="/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1/"
BAK="SharedPtrLeakChecker/original"
SRC="$PWD/SharedPtrLeakChecker"

FILES="vector map string functional set algorithm __string memory __tree"

for f in $FILES; do
    echo $f
    SRC_FILE="$SRC/$f"
    DEST_FILE="$DEST/$f"
    ls -l "$SRC_FILE"
    ls -l "$DEST_FILE"

    if [ ! -L $DEST_FILE ]; then
        sudo mv "$DEST_FILE" "$BAK"
        sudo ln -s "$SRC_FILE" "$DEST_FILE"
    fi
done

