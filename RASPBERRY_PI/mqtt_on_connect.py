#_______________________________________________________________________________________________________________________________#
#_______________________________________________________DESCRIPTION:____________________________________________________________#
#
# --> Module that handles the callback function of the MQTT Client.
#
# --> The callback for when the client receives a CONNACK (Connection Aknowleged) response from the server.
#
#_______________________________________________________________________________________________________________________________#

#_______________________________________________________________________________________________________________________________#
#_____________________________________________________IMPORT_LIBRARIES__________________________________________________________#
import paho.mqtt.client as mqtt
import time

#_______________________________________________________________________________________________________________________________#
#______________________________________________________IMPORT MODULES___________________________________________________________#
from mqtt_dictionary import mqtt_dictionary                     # Module that stores the most important info about MQTT Client status.                  
from mqtt_topic_subscribe import mqtt_topic_subscribe           # Module that handles:  MQTT Client subscribe to MQTT Topics on MQTT Server.

#_______________________________________________________________________________________________________________________________#
#_____________________________________________________GLOBAL_VARIABLES__________________________________________________________#

# --> TOPICS:
# MQTT_TOPIC_TEMPERATURE_CONTROL = 'RaspPi/tempControl'
# MQTT_TOPIC_CHECK_STATUS = 'RaspPi/ScriptConToMQTT'

# --> GLOBAL VARIABLES:

#_______________________________________________________________________________________________________________________________#
#_______________________________________________________MAIN_FUNCTION___________________________________________________________#

class mqtt_on_connect():

    def on_connect(client,userdata,flags,rc):
        try:
            if rc == 0:
                connectionReturnCodes = mqtt_dictionary.mqtt_dictionary()
                client.connected_flag = True 
                print(f" <OK> [mqtt_on_connect]  Connected with result code: {rc} - {connectionReturnCodes[rc]}    ")

                time.sleep(1)

                topic_subscriber = mqtt_topic_subscribe()       # Create an instance of mqtt_topic_subscribe
                topic_subscriber.topic_subscribe(client, rc)    # Call the topic_subscribe method on the instance

            else:
                client.connected_flag = False
                connectionReturnCodes = mqtt_dictionary.mqtt_dictionary()
                if rc in mqtt_dictionary.mqtt_dictionary():
                    print(f" <!> [mqtt_on_connect]  Could not connect with code:{rc} - {connectionReturnCodes[rc]}     ")

        except TimeoutError :
            print(f" <!>  [mqtt_on_connect]   --> TimeoutError <!>")
            pass

        except BlockingIOError:
            print(f" <!>  [mqtt_on_connect]   --> BlockingIOError  <!>")
            pass

        except None as e:
            print(f" <!>  [mqtt_on_connect]  Passed on None type error <!>")
            pass

        except Exception as e:
            print(f" <!> [mqtt_on_connect]  Could not connect. {e}     ")

#_______________________________________________________________________________________________________________________________#
#________________________________________________________RETURN_AREA____________________________________________________________#


#_______________________________________________________________________________________________________________________________#
#________________________________________________________ DEBUG_AREA____________________________________________________________#