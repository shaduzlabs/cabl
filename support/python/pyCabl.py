from pycabl import *
import numpy as np
import time
import os, sys



class CablClient:
    'The cabl client class'

    def __init__(self):
        print 'Constructor'
        self.client = Client(self.onInitDevice, self.onRender, self.onDisconnect)
        self.client.onButtonChanged(self.onButtonChanged)
        self.client.onPadChanged(self.onPadChanged)
        self.client.onEncoderChanged(self.onEncoderChanged)
        self.client.onKeyChanged(self.onKeyChanged)
        self.drawingContext = None
        self.drawingBuffer = None

    def __del__(self):
        'Destructor'
        del self.client
        print "Destructor"

    def onInitDevice(self):
        'Called after a device is connected'
        self.client.setLedPad(Pad.Pad1, ColorRGB(12,0,0))
        self.client.setLedPad(Pad.Pad2, ColorRGB(48,0,0))
        self.client.setLedPad(Pad.Pad3, ColorRGB(80,0,0))
        self.client.setLedPad(Pad.Pad4, ColorRGB(127,0,0))
        self.client.setLedPad(Pad.Pad5, ColorRGB(0,12,0))
        self.client.setLedPad(Pad.Pad6, ColorRGB(0,48,0))
        self.client.setLedPad(Pad.Pad7, ColorRGB(0,80,0))
        self.client.setLedPad(Pad.Pad8, ColorRGB(0,127,0))
        self.client.setLedPad(Pad.Pad9, ColorRGB(0,0,12))
        self.client.setLedPad(Pad.Pad10, ColorRGB(0,0,48))
        self.client.setLedPad(Pad.Pad11, ColorRGB(0,0,80))
        self.client.setLedPad(Pad.Pad12, ColorRGB(0,0,127))
        self.client.setLedPad(Pad.Pad13, ColorRGB(12,12,12))
        self.client.setLedPad(Pad.Pad14, ColorRGB(48,48,48))
        self.client.setLedPad(Pad.Pad15, ColorRGB(80,80,80))
        self.client.setLedPad(Pad.Pad16, ColorRGB(127,127,127))

        display1 = self.client.displayGraphic(0)
        display2 = self.client.displayGraphic(1)

        display1.white()
        display2.black()
        display1.setPixel(5,5,Pixel.Invert)
        display1.setPixel(5,6,Pixel.White)
        display1.setPixel(5,7,Pixel.Black)
        display2.setPixel(5,5,Pixel.Invert)
        display2.setPixel(5,6,Pixel.White)
        display2.setPixel(5,7,Pixel.Black)

#        self.drawingContext = self.client.getDrawingContext(0)
#        self.drawingBuffer = bytearray(self.drawingContext.getSize())

#        data = self.make_frame(0)
#        indexDest = 0
#        for col in range(0,H):
#            for row in range(0,W):
#                self.drawingBuffer[indexDest+1] = 0x1F & (data[row,col][2])
#                self.drawingBuffer[indexDest+1] |= 0xE0 & (data[row,col][1] << 4)
#                self.drawingBuffer[indexDest+0] = 0x07 & (data[row,col][1] >> 3)
#                self.drawingBuffer[indexDest+0] |= 0xF8 & (data[row,col][0] << 3)
#                indexDest += 2

        print( "Device connected" )
        return

    def onRender(self):
        'Called periodically while the device is connected'
        print( "Render!" )
#        if self.drawingContext:
#            self.drawingContext.write(self.drawingBuffer)

    def onDisconnect(self):
        'Called when a known device is disconnected'
        print( "Device disconnected" )

    def enumerateDevices(self):
        'Returns a list of the known devices detected'
        deviceDescriptors = self.client.enumerateDevices()
        print "Found " + str(len(deviceDescriptors)) + " known devices:"
        for dd in deviceDescriptors:
            print "  " + str(dd)
        return deviceDescriptors

    def onButtonChanged(self, button, state, shiftPressed):
        shiftStr = " + SHIFT" if shiftPressed else ""
        print(str(button) + ": " + str(state) + shiftStr)

    def onPadChanged(self, pad, value, shiftPressed):
        shiftStr = " + SHIFT" if shiftPressed else ""
        print(str(pad) + ": " + str(value) + shiftStr)

    def onKeyChanged(self, key, value, shiftPressed):
        shiftStr = " + SHIFT" if shiftPressed else ""
        print(str(key) + ": " + str(value) + shiftStr)

    def onEncoderChanged(self, encoder, increasing, shiftPressed):
        shiftStr = " + SHIFT" if shiftPressed else ""
        increasingStr = " ++ " if increasing else "--"
        print(str(encoder) + ": " + increasingStr + shiftStr)

theClient = CablClient()
theClient.enumerateDevices()

while True:
    time.sleep(1)
