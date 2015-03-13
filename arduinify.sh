#!/bin/bash
rm -f arduino/kompletIO.zip
mkdir -p arduino/kompletIO
cd src
find . -not -path '*/\.*' -type f -exec bash -c 'file=${1#./}; cp "$file" "../arduino/kompletIO/${file//\//_}"' _ '{}' \;
cd ..
cp -r support/arduino/* arduino/kompletIO/.
cd arduino
zip -9 -r kompletIO.zip kompletIO
rm -rf kompletIO
cd ..
