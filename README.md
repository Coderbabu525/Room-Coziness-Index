# Room-Coziness-Index
A room coziness index is calculated using the IoT sensor values of Temperature, Sound and Light Sensors. Raspberry PI is used as the gateway for arduino UNO sensors and Node-Red was used to implement the MQTT protocol and storing the data into a MongoDB database. The data was read from the database using a javascript backend and calculated the coziness index by giving assigned weight of each of the sensor values.
Finally the room coziness along with the other sensor values were shown in a dashboard using API in node-red
