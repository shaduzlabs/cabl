from pycabl import *
import numpy as np
import time
import os, sys



class CablClient:
    'The cabl client class'

    def __init__(self):
        self.client = Client()
        self.drawingContext = None
        self.drawingBuffer = None

    def __del__(self):
        'Destructor'
        del self.client
        print "Destructor"

    def run(self):
        'Start the client loop'
        self.client.run()
        print "Client started"

    def stop(self):
        'Stop the client loop'
        self.client.stop()
        print "Client has been stopped."

    def onConnect(self):
        'Called when a known device is connected'
        self.client.setLedPad(Pad.Pad1, LedColor(0,120,0))
        self.drawingContext = self.client.getDrawingContext(0)
        self.drawingBuffer = bytearray(self.drawingContext.getSize())

        data = self.make_frame(0)
        indexDest = 0
        for col in range(0,H):
            for row in range(0,W):
                self.drawingBuffer[indexDest+1] = 0x1F & (data[row,col][2])
                self.drawingBuffer[indexDest+1] |= 0xE0 & (data[row,col][1] << 4)
                self.drawingBuffer[indexDest+0] = 0x07 & (data[row,col][1] >> 3)
                self.drawingBuffer[indexDest+0] |= 0xF8 & (data[row,col][0] << 3)
                indexDest += 2

        print( "Device connected" )

    def onTick(self):
        'Called periodically while the device is connected'
        if self.drawingContext:
            self.drawingContext.write(self.drawingBuffer)

    def onDisconnect(self):
        'Called when a known device is disconnected'
        print( "Device disconnected" )
        discoverAndConnect()

    def enumerateDevices(self):
        'Returns a list of the known devices detected'
        deviceDescriptors = self.client.enumerateDevices()
        print "Found " + str(len(deviceDescriptors)) + " known devices:"
        for dd in deviceDescriptors:
            print "  " + str(dd)
        return deviceDescriptors

    def connect(self, deviceDescriptor):
        'Connect to a device. Returns TRUE if the connection succeeds, FALSE otherwise'
        deviceName = deviceDescriptor.getName()
        deviceSerialNumber = deviceDescriptor.getSerialNumber()
        print "Connecting to " + deviceName + " (" + deviceSerialNumber + ")\n"
        self.client.connect(deviceDescriptor)

    def discoverAndConnect(self):
        'Detect known devices and connect to the first device of the list'
        deviceDescriptors = self.enumerateDevices()
        while(len(deviceDescriptors)<=0):
            print("No devices found, retrying in 5 seconds.")
            time.sleep(5)
            deviceDescriptors = self.enumerateDevices()
        self.connect(deviceDescriptors[0])


theClient = CablClient()
theClient.run()
theClient.discoverAndConnect()

while True:
    time.sleep(1)
