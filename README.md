# Duck Duck SOC...
If you have reached this page it is because you want to build your rubber ducky notifcation thingie uber TACTIcool tool you received from the Palo Alto Networks booth at the Black Hat USA 2019 show! *gasping for breath*

#### Purpose:
The purpose of this development is to program your rubber ducky (SOC Duck) and build a web server. With the web server up and listening you can then have your Next-Generation Firewall send configurable alerts to provide you with a visual queue. This is intended as a gadget and not a production tool (see [Security Warning](#security-warning) below).

#### Security Warning
You will be building and deploying a **HTTP Server**. This means everything sent and received will be in the clear. Within the documentation are additional notes and references if you choose to build a TLS-enabled web server. Please be conscious of the network this would be living on. If you do manage to deploy this securely and use this in your Security Operations Center please drop us a note!

#### Requirements and Dependencies:
* The sketchbooks were coded using Arduino
* Understanding of your ESP32 schematic and GPIO pinouts (to power the blinkie thingZ!)
* Basic understanding of C/C++ (Amazing tutorials on the [Programming ELectronics Acamdemy YouTube Channel](https://www.youtube.com/user/OSHJunkies)
* Have FUN and remember to take breaks!

#### Getting Started & Documentation:
* If you have never played with ES32/Arduino/IDE, then [START HERE](https://github.com/espressif/arduino-esp32) with this great guide for installing Arduino IDE for your respective system.

* [Official Documentation for ESP-IDF (Espressif IoT Development Framework) ](https://docs.espressif.com/projects/esp-idf/en/latest/get-started/)

## Support Policy
The code and templates in the repo are released under an as-is, best effort, support policy. These scripts should be seen as community supported and Palo Alto Networks will contribute our expertise as and when possible. We do not provide technical support or help in using or troubleshooting the components of the project through our normal support options such as
Palo Alto Networks support teams, or ASC (Authorized Support Centers) partners and backline support options. The underlying product used (the VM-Series firewall) by the scripts or templates are still supported, but the support is only for the product functionality and not for help in deploying or using the template or script itself. Unless explicitly tagged, all projects or work posted in our GitHub repository (at https://github.com/PaloAltoNetworks) or sites other than our official Downloads page on https://support.paloaltonetworks.com are provided under the best effort policy.
