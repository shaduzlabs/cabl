#!/bin/bash
set -x
rm -f $1/$2/k-IO.zip
rm -rf $1/$2/k-IO/*
mkdir -p $1/$2/k-IO
cp -r $1/inc/* $1/$2/k-IO/
cp -r $1/src/* $1/$2/k-IO/
cp $1/support/arduino/k-IO.h $1/$2/k-IO/k-IO.h
cp $1/support/arduino/stl-arduino.h $1/$2/k-IO/util/stl-arduino.h
rm -rf $1/$2/k-IO/app
rm -rf $1/$2/k-IO/comm/drivers/HIDAPI
rm -rf $1/$2/k-IO/comm/drivers/LibUSB
find $1/$2/k-IO -type f -name *.cpp -print0 | xargs -0 -I%%% mv %%% $1/$2/k-IO/
zip -9 -r $1/$2/k-IO.zip $1/$2/k-IO
