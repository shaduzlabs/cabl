from pycabl import *
import time
import os, sys



class CablClient:
    'The cabl client class'

    def __init__(self, discoveryPolicy):
        'Constructor'
        self.client = Client(self.onInitDevice, self.onRender, self.onDisconnect, discoveryPolicy)
        self.client.onButtonChanged(self.onButtonChanged)
        self.client.onEncoderChanged(self.onEncoderChanged)
        self.client.onControlChanged(self.onControlChanged)
        self.client.onKeyChanged(self.onKeyChanged)

    def __del__(self):
        'Destructor'
        del self.client

    def onInitDevice(self):
        'Called after a device is connected'

    def onRender(self):
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

    def onButtonChanged(self, button, state, shiftPressed):
        self.client.setLedButton(button, Color(255) if state else Color(0) )
        shiftStr = " + SHIFT" if shiftPressed else ""
        print(str(button) + ": " + str(state) + shiftStr)
        self.client.updateDevice()

    def onControlChanged(self, control, value, shiftPressed):
        shiftStr = " + SHIFT" if shiftPressed else ""
        print(str(control) + ": " + str(value) + shiftStr)
        self.client.ledArray(control).setValue(value, Color(255,0,255), Alignment.Left)
        self.client.updateDevice()

    def onKeyChanged(self, key, value, shiftPressed):
        shiftStr = " + SHIFT" if shiftPressed else ""
        print(str(key) + ": " + str(value) + shiftStr)
        self.client.updateDevice()

    def onEncoderChanged(self, encoder, increasing, shiftPressed):
        shiftStr = " + SHIFT" if shiftPressed else ""
        increasingStr = " ++ " if increasing else "--"
        print(str(encoder) + ": " + increasingStr + shiftStr)
        self.client.updateDevice()

"""
The client will connect to any supported device matching the discovery policy. The default discovery
policy will connect to any of the supported devices.
"""
theClient = CablClient(DiscoveryPolicy("*"))

while True:
    time.sleep(1)
