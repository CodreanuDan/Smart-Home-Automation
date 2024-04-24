#_______________________________________________________________________________________________________________________________#
#_______________________________________________________DESCRIPTION:____________________________________________________________#
#
# --> Module that handles the callback function of the MQTT Client.
#
# --> The callback for when a PUBLISH message is received from the server.
#
# --> Handles the  esp_received_msg.json reads message from topic and if needed, dumps it into JSON File
#
#_______________________________________________________________________________________________________________________________#

#_______________________________________________________________________________________________________________________________#
#_____________________________________________________IMPORT_LIBRARIES__________________________________________________________#
import paho.mqtt.client as mqtt
import time
import json
import traceback

#_______________________________________________________________________________________________________________________________#
#______________________________________________________IMPORT MODULES___________________________________________________________#
from on_message_json_dump import on_message_json_dump  # Module responsable for JSON Dump of the values that came from topics, so that they can be accsesed with ease by other modules(mqtt_topic_subscribe).


#_______________________________________________________________________________________________________________________________#
#_____________________________________________________GLOBAL_VARIABLES__________________________________________________________#

#_______________________________________________________________________________________________________________________________#
#_______________________________________________________MAIN_FUNCTION___________________________________________________________#

class mqtt_on_message():

    # The callback for when a PUBLISH message is received from the server.
    def on_message(client,userdata,msg):

        try:
            print(f" <OK> [mqtt_on_message]   -msg.topic: {msg.topic} -Payload: {msg.payload}    ")
            
            #-------------------------JSON_HANDLERS-----------------------------#
            
            """
                JSON handlers for topics coming from ESP to RaspPI : ESP --> Topic --> RaspPI Python program --> JSON
                Dumps received message from topic to json file : esp_received_msg.json

            """
            message_topic_list = ["esp32/no.1/roomTemp", "esp32/no.1/airHum", "esp32/no.1/sysState",
                                   "RaspPi/esp32/no.1/outputRed", "RaspPi/esp32/no.1/Reset",
                                   "RaspPi/esp32/no.1/temperatureControl", "RaspPi/esp32/no.1/setLocation"]
            try:
                if msg.payload == None or msg.topic == None:
                    print(f" <!> [mqtt_on_message]   Received None type message!  {e}    ")
                    pass
                else:
                    message_topic = msg.topic

                    #   TOPIC: "esp32/no.1/roomTemp"
                    if message_topic == "esp32/no.1/roomTemp":
                        jsonDumperAirTemp = on_message_json_dump()
                        jsonDumperAirTemp.json_dump_airTemp(msg)

                    #   TOPIC: "esp32/no.1/airHum"
                    elif message_topic == "esp32/no.1/airHum":
                        jsonDumperAirHum = on_message_json_dump()
                        jsonDumperAirHum.json_dump_airHum(msg)

                    #   TOPIC: "esp32/no.1/sysState"
                    elif message_topic == "esp32/no.1/sysState":
                        jsonDumperSysState = on_message_json_dump()
                        jsonDumperSysState.json_dump_sysState(msg)

                    #  TOPIC: "esp32/no.1/airPressure"
                    elif message_topic == "esp32/no.1/airPressure":
                        jsonDumperAirPressure = on_message_json_dump()
                        jsonDumperAirPressure.json_dump_airPressure(msg)

                        """
                        JSON handlers for coming from DASHBOARD to RaspPI : Dashboard --> Topic --> RaspPI Python program --> JSON
                        Dumps received message from topic to json file : esp_received_msg.json

                        """

                    #   TOPIC: RaspPi/esp32/no.1/temperatureControl
                    elif message_topic == "RaspPi/esp32/no.1/temperatureControl":
                        jsonDumperSysState = on_message_json_dump()
                        jsonDumperSysState.json_dump_tempCtrl(msg)

                    #   TOPIC: RaspPi/esp32/no.1/setLocation
                    elif message_topic == "RaspPi/esp32/no.1/setLocation":
                        jsonDumperSetLocation= on_message_json_dump()
                        jsonDumperSetLocation.json_dump_setLocation(msg)

                    #   TOPIC: RaspPi/esp32/no.1/outputRed
                    # elif message_topic == "RaspPi/esp32/no.1/outputRed":
                        # jsonDumperLedOnOff = on_message_json_dump()
                        # jsonDumperLedOnOff.json_dump_ledOnOff(msg)
                    
                    #   TOPIC: RaspPi/esp32/no.1/Reset
                    # elif message_topic == "RaspPi/esp32/no.1/Reset":
                        # jsonDumperResetButton = on_message_json_dump()
                        # jsonDumperResetButton.json_dump_resetButton(msg)

            except None as e:
                print(f" <!> [mqtt_on_message]   Received None type message!  {e}    ")

        except TimeoutError :
            print(f" <!>  [mqtt_on_message]   --> TimeoutError <!>")
            pass

        except BlockingIOError:
            print(f" <!>  [mqtt_on_message]   --> BlockingIOError  <!>")
            pass

        except None as e:
            print(f" <!>  [mqtt_on_message]  Passed on None type error <!>")
            pass

        except Exception as e:
            print(f" <!> [mqtt_on_message]   Could not recive messages!  {e}    ")
            #############################################################
            #|                         ERROR LOG                       |#
            #############################################################
            if not (isinstance(e, TypeError) and "'NoneType' object" in str(e)):
                with open("exceptions_main.log", "a") as logfile:
                    traceback.print_exc(file=logfile)



#_______________________________________________________________________________________________________________________________#
#________________________________________________________RETURN_AREA____________________________________________________________#

#_______________________________________________________________________________________________________________________________#
#________________________________________________________ DEBUG_AREA____________________________________________________________#