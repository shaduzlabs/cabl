#!/bin/bash
rm -f arduino/k-IO.zip
rm -rf arduino/k-IO/*
mkdir -p arduino/k-IO
cp -r inc/* arduino/k-IO/
cp -r src/* arduino/k-IO/
cp -r support/arduino/k-IO.h arduino/k-IO/k-IO.h
rm -rf arduino/k-IO/app
rm -rf arduino/k-IO/comm/drivers/HIDAPI
rm -rf arduino/k-IO/comm/drivers/LibUSB
cd arduino
find . -type f -name *.cpp -print0 | xargs -0 -I%%% mv %%% k-IO/
zip -9 -r k-IO.zip k-IO
cd ..
