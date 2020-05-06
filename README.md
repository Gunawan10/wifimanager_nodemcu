NodeMCU 1.0 (ESP-12E Module) WifiManager
========================================

In the IoT world, network management is needed because you no longer have to hard-code your network credentials (SSID and password). Your system will automatically join a known network or set up an Access Point that you can use to configure network credentials. In this case, i'm trying to make my own network manager for NodeMCU. I hope this example project can help you.

What works
----------

 * Join a known network via HTTP request (POST).
 * Can see the connection status via HTTP request (GET).
 * Connect to a previously saved Access Point
 * Save wifi configuration
 * Check if the network have internet connection
 * Has an indicator to know that the system needs to be connected to the network, is connecting the network and the system is connected to the network
 

Materials
---------

 * 1 NodeMCU 1.0 (ESP-12E Module)
 * 1 Diode LED (Red or Green)
 * 1 Resistor 220 Ohm 
 * 1 Mini BreadBoard
 * Wired
 
Required libraries
------------------

**ESP8266 (Latest version)**

* In the Arduino IDE, navigate to *File > Preferences*
* Enter ``http://arduino.esp8266.com/stable/package_esp8266com_index.json`` into the “Additional Boards Manager URLs” field as shown in the figure below. Then, click the “OK” button
* Open the Boards Manager. Go to *Tools > Board > Boards Manager..*
* Search for **ESP8266** and press install button for the **ESP8266 by ESP8266 Community**

**ESP8266Ping (Latest version)**

* Download library [here](https://github.com/dancol90/ESP8266Ping)
* In the Arduino IDE, navigate to *Sketch > Include Library > Add .Zip Library*. You will be prompted to select the library you would like to add. Navigate to the .zip file's location and open it.

**ArduinoJson (Version 5.11.2)**

In the Arduino IDE, navigate to *Sketch > Include Library > Manage Libraries*. In order to install ArduinoJson, search for "ArduinoJson", scroll the list to find it and click on it.

Installation
------------

 * Download the **sketch_wifimanager.ino** file in this repository
 * Open Arduino IDE, install the required library
 * Connect each component with a cable (cabling). You can see the cabling image in this repository
 * Connect the NodeMCU component with a USB cable to the computer
 * In the Arduino IDE, navigate to *Tools > Board* and select *NodeMCU 1.0 (ESP-12E Module)*
 * Open the **sketch_wifimanager.ino** file. In the Arduino IDE, navigate to *File > Open* and select **sketch_wifimanager.ino** file and click "Upload"
 * If successful uploads, navigate to *Tools > Serial Monitor*. You can use the **Serial Monitor** to debug the Arduino Software Sketch or to see data sent by a working Sketch.
 
How it Works
------------
 * When your NodeMCU starts up, it sets it up in Station mode and tries to connect to a previously saved Access Point
 * if this is unsuccessful (or no previous network saved) it moves the ESP into Access Point mode and spins up a DNS and WebServer (default ip 192.168.4.1)
 * using any wifi enabled device (computer, phone, tablet) connect to the newly created Access Point
 * To join a known network, you can see in the documentation how it works
 * After that, NodeMCU will try to connect. If successful, the system will save the network credential configuration. If not, reconnect to AP and reconfigure.
 * To see the connection status, you can see in the documentation how it works

Documentation
-------------

**Endpoint**

 * ``http://IP_ADDRESS/setwifi`` for Join a known network via HTTP request (POST),
 * ``http://IP_ADDRESS/status`` for See the connection status via HTTP request (GET)
 
**Join a known network**

 * You can use any application as long as you can make HTTP POST requests.
 * Using endpoint ``http://IP_ADDRESS/setwifi``. You can get ip address from Serial Monitor
 * Form Data for Post, ``ssid`` and ``pass``
 
**See the connection status**
 
 * You can use any application as long as you can make HTTP GET requests.
 * Using endpoint ``http://IP_ADDRESS/status``. You can get ip address from Serial Monitor
 
**LED light signal code**
 
 * Needs to be connected to the network, the lamp will turn on and turn off for ``1 second`` continuously
 * Is connecting the network, the lamp will turn on and off for ``0.25 seconds`` continuously
 * The system is connected to the network, the light will turn on
