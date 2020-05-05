NodeMCU 1.0 (ESP-12E Module) WifiManager
========================================

In the IoT world, network management is needed because you no longer have to hard-code your network credentials (SSID and password). Your system will automatically join a known network or set up an Access Point that you can use to configure network credentials. In this case, i'm trying to make my own network manager at NodeMCU. I hope this example project can help you.

What works
----------

 * Connection configuration via HTTP request (POST).
 * Can see the connection status via HTTP request (GET).
 * Connect to a previously saved Access Point
 * Save wifi configuration
 * Check if the network have internet connection
 

Materials
---------

 * 1 NodeMCU 1.0 (ESP-12E Module)
 * 1 Diode LED (Red or Green)
 * 1 Resistor 220 Ohm 
 * 1 Mini BreadBoard
 * Wired
 
Required libraries
------------------

ESP8266 (Latest version)
~~~~~~~~~~~~~~~~~~~~~~~~

* In the Arduino IDE, navigate to *File > Preferences*
* Enter ``http://arduino.esp8266.com/stable/package_esp8266com_index.json`` into the “Additional Boards Manager URLs” field as shown in the figure below. Then, click the “OK” button
* Open the Boards Manager. Go to *Tools > Board > Boards Manager..*
* Search for **ESP8266** and press install button for the **ESP8266 by ESP8266 Community**

ESP8266Ping (Latest version)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

* Download library `here <https://github.com/dancol90/ESP8266Ping>`__ . 
* In the Arduino IDE, navigate to *Sketch > Include Library > Add .Zip Library*. You will be prompted to select the library you would like to add. Navigate to the .zip file's location and open it.

ArduinoJson (Version 5.11.2)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

In the Arduino IDE, navigate to *Sketch > Include Library > Manage Libraries*. In order to install ArduinoJson, search for "ArduinoJson", scroll the list to find it and click on it.

Installation
------------

 * Download the sketch_wifimanager.ino file in this repository
 * Open Arduino IDE, install the required library
 * Connect each component with a cable (cabling). You can see the cabling image in this repository
 * Connect the NodeMCU component with a USB cable to the computer
 * Open the **sketch_wifimanager.ino** file. In the Arduino IDE, navigate to *File > Open* and select **sketch_wifimanager.ino** file and click "Upload"
