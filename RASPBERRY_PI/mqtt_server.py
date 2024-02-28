#_______________________________________________________________________________________________________________________________#
#_______________________________________________________DESCRIPTION:____________________________________________________________#
#
# --> Module that handles the communication with the MQTT Server.
#
# --> Responsable for connection to MQTT Broker.
#
# --> Contanis the client loop.
#
# --> Made for controlling the featuers of the MQTT Server as wanted by the creator
#
#_______________________________________________________________________________________________________________________________#

#_______________________________________________________________________________________________________________________________#
#_____________________________________________________IMPORT_LIBRARIES__________________________________________________________#
import paho.mqtt.client as mqtt
import time

#_______________________________________________________________________________________________________________________________#
#______________________________________________________IMPORT MODULES___________________________________________________________#
from mqtt_on_connect import mqtt_on_connect             # Module that handles the callback function of the MQTT Client.
from mqtt_on_message import mqtt_on_message             # Module that handles the callback function of the MQTT Client for when a PUBLISH message is received from the server.)
from mqtt_dictionary import mqtt_dictionary             # Module that stores the most important info about MQTT Client status.     


#_______________________________________________________________________________________________________________________________#
#_____________________________________________________GLOBAL_VARIABLES__________________________________________________________#
MQTT_BROKER = '169.254.175.150'
MQTT_PORT = 1883

mqtt.Client.connected_flag=False    # Create flag in class to verify connection to MQTT Broker.

#_______________________________________________________________________________________________________________________________#
#_______________________________________________________MAIN_FUNCTION___________________________________________________________#

class mqtt_connect():

   
    print(" <OK> [mqtt_server][MAIN]  Connection to MQTT Broker ",MQTT_BROKER)
    try:

        client = mqtt.Client()
        client.on_connect = mqtt_on_connect.on_connect
        client.on_message = mqtt_on_message.on_message

        while True:

            client.loop_start()
            client.connect(MQTT_BROKER,MQTT_PORT)
            
            print(f" <OK> [mqtt_server][MAIN]  Connecting to MQTT Broker")
            while not client.connected_flag:                                # Wait in loop until connection succsessfull.
                print(" <!> [mqtt_server]   In wait loop ")
            print(" <OK> [mqtt_server][MAIN]   In Main Loop ")


            time.sleep(2)
            print('\n')


            print("_____________________________________\n")
    
    except KeyboardInterrupt:
            client.publish('RaspPi/WiFiStatus',"Disconnected")
            client.publish('RaspPi/ScriptConToMQTT',"Stopped")
            client.publish('RaspPi/CurrentTime','   -- : -- : --    ')
            print("\n[mqtt_raspi_status]   KeyboardInterrupt: Exiting the current time loop.")
            client.loop_stop()                                              # Stop loop 
            client.disconnect()                                             # Disconnect

    except Exception as e:
        print(f" <!> [mqtt_server]  Could not connect to MQTT Broker. {e}    <!>")  
        exit(1)


   
#_______________________________________________________________________________________________________________________________#
#________________________________________________________RETURN_AREA____________________________________________________________#

#_______________________________________________________________________________________________________________________________#
#________________________________________________________ DEBUG_AREA____________________________________________________________#
    

    