from pycabl import *
import time

class CablClient:
    'The cabl client class'

    def __init__(self):
        self.client = Client()
        self.client.registerCallbacks(self.onConnect, self.onTick, self.onDisconnect)

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
        self.client.setLedPad(Pad.Pad2, LedColor(0,110,0))
        self.client.setLedPad(Pad.Pad3, LedColor(0,100,0))
        self.client.setLedPad(Pad.Pad4, LedColor(0,95, 0))
        self.client.setLedPad(Pad.Pad5, LedColor(0,90, 0))
        self.client.setLedPad(Pad.Pad6, LedColor(0,85, 0))
        self.client.setLedPad(Pad.Pad7, LedColor(0,80, 0))
        self.client.setLedPad(Pad.Pad8, LedColor(0,75, 0))
        self.client.setLedPad(Pad.Pad9, LedColor(0,70, 0))
        self.client.setLedPad(Pad.Pad10,LedColor(0,60, 0))
        self.client.setLedPad(Pad.Pad11,LedColor(0,50, 0))
        self.client.setLedPad(Pad.Pad12,LedColor(0,40, 0))
        self.client.setLedPad(Pad.Pad13,LedColor(0,35, 0))
        self.client.setLedPad(Pad.Pad14,LedColor(0,30, 0))
        self.client.setLedPad(Pad.Pad15,LedColor(0,20, 0))
        self.client.setLedPad(Pad.Pad16,LedColor(0,10, 0))
        print( "Device connected" )

    def onTick(self):
        'Called periodically while the device is connected'

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

    def connect(self, deviceDescriptor):
        'Connect to a device. Returns TRUE if the connection succeeds, FALSE otherwise'
        deviceName = deviceDescriptor.getName()
        deviceSerialNumber = deviceDescriptor.getSerialNumber()
        print "Connecting to " + deviceName + " (" + deviceSerialNumber + ")\n"
        self.client.connect(deviceDescriptor)

    def discoverAndConnect(self):
        'Detect known devices and connect to the first device of the list'
        deviceDescriptors = self.enumerateDevices()
        if(len(deviceDescriptors)>0):
            self.connect(deviceDescriptors[0])


#time.sleep(12)
theClient = CablClient()
theClient.run()
theClient.discoverAndConnect()


while True:
    time.sleep(1)
