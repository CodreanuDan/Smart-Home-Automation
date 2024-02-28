import paho.mqtt.client as mqtt
import RPi.GPIO as GPIO
import time

# MQTT Settings
MQTT_BROKER = '169.254.175.150'
MQTT_PORT = 1883
MQTT_TOPIC_CHECK_STATUS = 'RaspPi/ScriptConToMQTT'

while True: 
    nr = 0
    # Callback when the client connects to the MQTT broker
    def on_connect(client, userdata, flags, rc):
        print(f"Connected with result code {rc}")
        client.subscribe(MQTT_TOPIC_CHECK_STATUS)
        
        # Publish a message to the topic when connected
        client.publish(MQTT_TOPIC_CHECK_STATUS, nr)
        print(f"Status message sent: {nr}")

    # Callback when a message is received from the MQTT broker
    def on_message(client, userdata, msg):
        # Process the received message if needed
        print(f"Received message on topic {msg.topic}: {msg.payload.decode()}")

    # Create MQTT client
    client = mqtt.Client()
    client.on_connect = on_connect
    client.on_message = on_message

    # Connect to MQTT broker
    client.connect(MQTT_BROKER, MQTT_PORT, 60)

    # Loop to keep the script running and processing MQTT messages
    nr = nr +1
    time.sleep(1)
    client.loop_forever()


# import paho.mqtt.client as mqtt  #import the client1
# import time

# def on_connect(client, userdata, flags, rc):
#     if rc==0:
#         client.connected_flag=True #set flag
#         print("connected OK")
#     else:
#         print("Bad connection Returned code=",rc)

# mqtt.Client.connected_flag=False#create flag in class
# broker='169.254.175.150'
# client = mqtt.Client("python1")             #create new instance 
# client.on_connect=on_connect  #bind call back function
# client.loop_start()
# print("Connecting to broker ",broker)
# client.connect(broker)      #connect to broker
# while not client.connected_flag: #wait in loop
#     print("In wait loop")
#     time.sleep(1)
# print("in Main Loop")
# client.loop_stop()    #Stop loop 
# client.disconnect() # disconnect