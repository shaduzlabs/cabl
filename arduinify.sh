#!/bin/bash
set -x
rm -f $1/$2/cabl.zip
rm -rf $1/$2/cabl/*
mkdir -p $1/$2/cabl
cp -r $1/inc/* $1/$2/cabl/
cp -r $1/src/* $1/$2/cabl/
cp $1/support/arduino/cabl.h $1/$2/cabl/cabl.h
cp $1/support/arduino/stl-arduino.h $1/$2/cabl/util/stl-arduino.h
rm -rf $1/$2/cabl/app
rm -rf $1/$2/cabl/comm/drivers/HIDAPI
rm -rf $1/$2/cabl/comm/drivers/LibUSB
rm -rf $1/$2/cabl/comm/drivers/MIDI
find $1/$2/cabl -type f -name *.cpp -print0 | xargs -0 -I%%% mv %%% $1/$2/cabl/
zip -9 -r $1/$2/cabl.zip $1/$2/cabl
