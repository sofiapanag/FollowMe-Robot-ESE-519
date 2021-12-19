# FINAL PROJECT - FOLLOW ME CART

## DEVPOST LINK
https://devpost.com/software/final-project-3opmc1/joins/zS8k2ynE-ASz_fqPURAMtQ

## Youtube LINK
https://youtu.be/UMGsdQlZP_E


## Compilation
- please find the main.c. Microchip Studio is really bloated hence we placed the whole project in repo. You may need to setup the uart functionalities.
- the controller folder contains necessary files for NodeMCU. You may nead to install the AdaFruit bluefruit library.


### BLUETOOTH

Node MCU - Level Shifter - Arduino


Node MCU SPI Pins
- CLK  - D5
- COPI - D6
- CIPO - D7
- CS   - D8
- IRQ  - D3
- RST  - D2
- Arduino - D1
- Arduino - D0
- Arduino - D4

How the Adafruit Bluetooth code works.

For our project, we utilized the controler.ino file in to receive sensor readings from the Adafruit app on the user's phone. 
Before the Bluetooth sends data, several commands in the controller.ino file are send to the Bluetooth.
These commands are listed below:
- ble.echo(false) - Disable echo command from Bluefruit
- ble.isConnected() - Ensure connection with bluetooth
- ble.isVersionAtLeast(MINIMUM_FIRMWARE_VERSION) - To check the firmware version on the Bluetooth
- ble.setMode(BLUEFRUIT_MODE_DATA) - To inform the Bluetooth device to stream sensor data

These commands are all sent in the setup function within arduino.

The key abstraction used in the code is the Adafruit_BluefruitLE_SPI class in the Adafruit_Bluetooth_SPI.h file.
This class maintains the necessary functions and pinouts needed to send and receive data from the Bluetooth.
The constructor is instantiated with the SPI pins listed above(Node MCU SPI pins). The m_rx_fifo and m_rx_buffer arse used to buffer incoming data. The packet size for our sensor readings in 15. The class also has a transmitt buffer (m_tx_buffer) and a counter (m_tx_count)  that ensures the packet to be sent is within maximum packet size limit. The class has two functions sendPacket and getPacket that send and receive packets from the Bluetooth.


The controller.ino file polls for the incoming packets in the main loop. Whenever data is received, it is parsed in the readPacket function. This function return perform the validity check for the message using a checksum. It returns the length of the message. 

Once the message validity has been identified, depending on the type of sensor reading, 4 bytes are parse and cast into a float type variable. We can then use this float time to perform calculations.
