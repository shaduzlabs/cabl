from pycabl import *
import gizeh
import numpy as np
import time
import os, sys
import  PIL
from PIL import Image

W,H = 1024,160
duration = 2
ncircles = 20 # Number of circles

class CablClient:
    'The cabl client class'

    def __init__(self):
        self.client = Client()
        self.client.registerCallbacks(self.onConnect, self.onTick, self.onDisconnect)
        self.drawingContext = None
        self.drawingBuffer = None

    def __del__(self):
        'Destructor'
        del self.client
        print "Destructor"

    def make_frame(self,t):
        im = Image.open("pac.bmp")
        print im.load()[0,0]
        print im.size
        return im.load()

    def make_frame2(self, t):

        surface = gizeh.Surface(W,H)

        for i in range(ncircles):
            angle = 2*np.pi*(1.0*i/ncircles+t/duration)
            center = W*( 0.5+ gizeh.polar2cart(0.1,angle))
            circle = gizeh.circle(r= W*(1.0-1.0*i/ncircles),
                                  xy= center, fill= (i%2,i%2,i%2))
            circle.draw(surface)

        return surface.get_npimage()

    def run(self):
        'Start the client loop'
        self.client.run()
        print "Client started"

    def stop(self):
        'Stop the client loop'
        self.client.stop()
        print "Client has been stopped."

    def onConnect2(self):
        'Called when a known device is connected'
        self.client.setLedPad(Pad.Pad1, LedColor(0,120,0))
        self.drawingContext = self.client.getDrawingContext(0)
        self.drawingBuffer = bytearray(self.drawingContext.getSize())

        data = self.make_frame(0)
        indexDest = 0
        for row in range(0,H):
            for col in range(0,W):
                self.drawingBuffer[indexDest] = 0x1F & (data[row][col][0])
                self.drawingBuffer[indexDest] |= 0xE0 & (data[row][col][1] << 5)
                self.drawingBuffer[indexDest+1] = 0x07 & (data[row][col][1] >> 3)
                self.drawingBuffer[indexDest+1] |= 0xF8 & (data[row][col][2] << 3)
                indexDest += 2

        print( "Device connected" )

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


#time.sleep(12)
theClient = CablClient()
theClient.run()
theClient.discoverAndConnect()

while True:
    time.sleep(1)
