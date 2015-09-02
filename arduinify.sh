#!/bin/bash
rm -f artifacts/arduino/k-IO.zip
rm -rf artifacts/arduino/k-IO/*
mkdir -p artifacts/arduino/k-IO
cp -r inc/* artifacts/arduino/k-IO/
cp -r src/* artifacts/arduino/k-IO/
cp -r support/arduino/k-IO.h artifacts/arduino/k-IO/k-IO.h
rm -rf artifacts/arduino/k-IO/app
rm -rf artifacts/arduino/k-IO/comm/drivers/HIDAPI
rm -rf artifacts/arduino/k-IO/comm/drivers/LibUSB
cd artifacts/arduino
find . -type f -name *.cpp -print0 | xargs -0 -I%%% mv %%% k-IO/
zip -9 -r k-IO.zip k-IO
cd ..
