from pycabl import *

class CablClient:
    'The cabl client class'

    def __init__(self):
        self.client = ClientSingle()
        self.client.registerCallbacks(self.onConnect, self.onTick, self.onDisconnect)
        self.device = None

    def run(self):
        'Start the client loop'
        self.client.stop()
        print "Client started"

    def stop(self):
        'Stop the client loop'
        self.client.stop()
        print "Client has been stopped."

    def onConnect(self):
        'Called when a known device is connected'
        print( "Device connected" )

    def onTick(self):
        'Called periodically while the device is connected'
        print( ".")

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


theClient = CablClient()
theClient.run()
theClient.discoverAndConnect()
theClient.stop()
