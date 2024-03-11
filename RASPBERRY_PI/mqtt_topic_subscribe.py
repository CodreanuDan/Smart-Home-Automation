#_______________________________________________________________________________________________________________________________#
#_______________________________________________________DESCRIPTION:____________________________________________________________#
#
# --> Module that handles:  MQTT Client subscribe to MQTT Topics on MQTT Server.
#
# --> Sablon pentru operatiunea de subscribe la MQTT Topics:
#
#
# try:
#     MQTT_TOPIC      = 
#     MQTT_TOPIC_SEND =
# 
#     client.subscribe(MQTT_TOPIC,2)
#     client.subscribe(MQTT_TOPIC_SEND,2)
# 
#     if rc == 0:
#         connectionReturnCodes = mqtt_dictionary.mqtt_dictionary()
#         print(f"[mqtt_topic_connect][]   Subscribed to topics: {MQTT_TOPIC, MQTT_TOPIC_SEND}, rc: {rc} - {connectionReturnCodes[rc]}.     <OK>")
#
#          # Create an instance 
#          # Call the method on the instance
#
#     else:
#         connectionReturnCodes = mqtt_dictionary.mqtt_dictionary()
#         print(f"[mqtt_topic_connect][]   Failed to subscribe to topics: {MQTT_TOPIC, MQTT_TOPIC_SEND}, rc: {rc} - {connectionReturnCodes[rc]}.     <!>") 
# except Exception as e:
#     MQTT_TOPIC = 
#     MQTT_TOPIC_SEND =
#     print(f"[mqtt_topic_connect]][]   Failed to subscribe to topics: {MQTT_TOPIC, MQTT_TOPIC_SEND}. {e}    <!>") 
#
# --> MQTT QoS: 0, 1, 2 (Quality of Service):
#
#           0 - Pub: Will send a message only once.  --> Sub: Might receive or might not receive the message.
#           1 - Pub: Will send a message at least once as long as an acknowledgement is received or the  --> Sub: It is likely to receive the message at least once 
#                    command to end the transmission is received.                                                 (it is possible that the message can be received more than once).
#           2 - Pub: Will send a message only once.  --> Sub: Will only receive the message once.
#_______________________________________________________________________________________________________________________________#

#_______________________________________________________________________________________________________________________________#
#_____________________________________________________IMPORT_LIBRARIES__________________________________________________________#
import paho.mqtt.client as mqtt
import time
import json


#_______________________________________________________________________________________________________________________________#
#______________________________________________________IMPORT MODULES___________________________________________________________#
from mqtt_dictionary   import mqtt_dictionary       # Dictionaty containing MQTT RC Codes
from mqtt_raspi_status import mqtt_raspi_status     # Class that contains functions that are retriving data from RaspPi
from mqtt_on_message   import mqtt_on_message       # Class that handles the reciving the messages and esp_received_msg.json

#_______________________________________________________________________________________________________________________________#
#_____________________________________________________GLOBAL_VARIABLES__________________________________________________________#

# --> TOPICS:

# a) Module 3 -RaspberryPi 4 TOPICS:

MQTT_TOPIC_CHECK_STATUS         = 'RaspPi/ScriptConToMQTT'
MQTT_TOPIC_WIFI                 = 'RaspPi/WiFiStatus'
MQTT_TOPIC_TIME                 = 'RaspPi/CurrentTime'

# b.1) Module 1 - ESP32/No.1 TOPICS:

ESP1_TOPIC_TEMPERATURE          = 'esp32/no.1/roomTemp'
ESP1_TOPIC_AIR_HUMIDITY         = 'esp32/no.1/airHum'
# ESP1_TOPIC_LED_CONTROL          = 'esp32/no.1/outputRed'
# ESP1_TOPIC_RESET                = 'esp32/no.1/Reset'
ESP1_TOPIC_SYS_STATE            = 'esp32/no.1/sysState'
ESP1_TOPIC_TEMP_CONTROL         = 'esp32/no.1/temperatureControl'

# b.2) Module 1 - ESP32/No.1 TOPICS to NodeRed Dashboard via RaspPi Python Program:

RPI_ESP1_TOPIC_TEMPERATURE      = 'RaspPi/esp32/no.1/roomTemp'
RPI_ESP1_TOPIC_AIR_HUMIDITY     = 'RaspPi/esp32/no.1/airHum'
# RPI_ESP1_TOPIC_LED_CONTROL      = 'RaspPi/esp32/no.1/outputRed'
# RPI_ESP1_TOPIC_RESET            = 'RaspPi/esp32/no.1/Reset'
RPI_ESP1_TOPIC_SYS_STATE        = 'RaspPi/esp32/no.1/sysState'
RPI_ESP1_TOPIC_TEMP_CONTROL     = 'RaspPi/esp32/no.1/temperatureControl'


# --> GLOBAL VARIABLES:


#_______________________________________________________________________________________________________________________________#
#_______________________________________________________MAIN_FUNCTION___________________________________________________________#

class mqtt_topic_subscribe():                   # Class that realises the connection to MQTT Topics. Inside each block are implemented functionalities based for each topic apart.
    
    """

    Function topic_subscribe(self,client,rc): is a function that subscribe to specific MQTT topics and then 
    takes the specific steps to read the data from the json and make connection between topics from the ESP 
    or Node Red Dasboard or diretly from the server script to either ESP or Node Red Dashboard
                                Server Script -> Topic -> Dashboard
                          ESP -> Topic -> Server Script -> Topic -> Dashboard
                          Dashboard -> Topic -> Server Script -> Topic -> ESP

    """
    def topic_subscribe(self,client,rc):        # The self parameter represents the instance of the class.

#_______________________________________________________________________________________________________________________________#
        
        # MQTT_TOPIC_CHECK_STATUS               *Server Script -> Topic -> Dashboard
        try:

            MQTT_TOPIC = MQTT_TOPIC_CHECK_STATUS
            client.subscribe(MQTT_TOPIC,2)

            if rc == 0:

                connectionReturnCodes = mqtt_dictionary.mqtt_dictionary()
                print(f" <OK> [mqtt_topic_connect][rpiStatus]   Subscribed to topic: {MQTT_TOPIC}, rc: {rc} - {connectionReturnCodes[rc]}.     ")

                msg_sender_instance = mqtt_raspi_status()                    # Create an instance 
                msg_sender_instance.script_conMQTT(client, MQTT_TOPIC)       # Call the msg_sender method on the instance

            else:

                connectionReturnCodes = mqtt_dictionary.mqtt_dictionary()
                print(f" <!> [mqtt_topic_connect][rpiStatus]   Failed to subscribe to topic: {MQTT_TOPIC}, rc: {rc} - {connectionReturnCodes[rc]}.    ") 
        except TimeoutError :
            print(f" <!> [mqtt_topic_connect][rpiStatus] --> TimeoutError <!>")
            pass

        except BlockingIOError:
            print(f" <!> [mqtt_topic_connect][rpiStatus] --> BlockingIOError  <!>")
            pass

        except None as e:
            print(f" <!> [mqtt_topic_connect][rpiStatus] Passed on None type error <!>")
            pass

        except Exception as e:
            MQTT_TOPIC = MQTT_TOPIC_CHECK_STATUS
            print(f" <!> [mqtt_topic_connect][rpiStatus]   Failed to subscribe to topic: {MQTT_TOPIC, MQTT_TOPIC_SEND}. {e}   ") 
#_______________________________________________________________________________________________________________________________#
            
        # MQTT_TOPIC_WIFI                       *Server Script -> Topic -> Dashboard
        try:

            MQTT_TOPIC = MQTT_TOPIC_WIFI 
            client.subscribe(MQTT_TOPIC,2)

            if rc == 0:

                connectionReturnCodes = mqtt_dictionary.mqtt_dictionary()
                print(f" <OK> [mqtt_topic_connect][wifi]   Subscribed to topic: {MQTT_TOPIC}, rc: {rc} - {connectionReturnCodes[rc]}.   ")
        
                wifi_status_instance = mqtt_raspi_status()                      # Create an instance 
                wifi_status_instance.wifi_status(client, MQTT_TOPIC)            # Call the method on the instance
        
            else:

                connectionReturnCodes = mqtt_dictionary.mqtt_dictionary()
                print(f" <!> [mqtt_topic_connect][wifi]   Failed to subscribe to topic: {MQTT_TOPIC}, rc: {rc} - {connectionReturnCodes[rc]}.    ") 
        except TimeoutError :
            print(f" <!> [mqtt_topic_connect][wifi] --> TimeoutError <!>")
            pass

        except BlockingIOError:
            print(f" <!> [mqtt_topic_connect][wifi] --> BlockingIOError  <!>")
            pass

        except None as e:
            print(f" <!> [mqtt_topic_connect][wifi] Passed on None type error <!>")
            pass

        except Exception as e:
            MQTT_TOPIC = MQTT_TOPIC_WIFI 
            print(f" <!> [mqtt_topic_connect][wifi]   Failed to subscribe to topic: {MQTT_TOPIC}. {e}   ") 
#_______________________________________________________________________________________________________________________________#
            
        # MQTT_TOPIC_TIME                       *Server Script -> Topic -> Dashboard
        try:

            MQTT_TOPIC = MQTT_TOPIC_TIME 
            client.subscribe(MQTT_TOPIC,2)

            if rc ==  0:

                connectionReturnCodes = mqtt_dictionary.mqtt_dictionary()
                print(f" <OK> [mqtt_topic_connect][rpiTime]   Subscribed to topic: {MQTT_TOPIC}, rc: {rc} - {connectionReturnCodes[rc]}.    ")
        
                get_current_time_instance = mqtt_raspi_status()                  # Create an instance 
                get_current_time_instance.get_current_time(client,MQTT_TOPIC)    # Call the method on the instance
            else:

                connectionReturnCodes = mqtt_dictionary.mqtt_dictionary()
                print(f" <!> [mqtt_topic_connect][rpiTime]   Failed to subscribe to topic: {MQTT_TOPIC}, rc: {rc} - {connectionReturnCodes[rc]}.    ")

        except TimeoutError :
            print(f" <!> [mqtt_topic_connect][rpiTime] --> TimeoutError <!>")
            pass

        except BlockingIOError:
            print(f" <!> [mqtt_topic_connect][rpiTime] --> BlockingIOError  <!>")
            pass

        except None as e:
            print(f" <!> [mqtt_topic_connect][rpiTime] Passed on None type error <!>")
            pass
 
        except Exception as e:
            MQTT_TOPIC = MQTT_TOPIC_TIME 
            print(f" <!> [mqtt_topic_connect][rpiTime]]   Failed to subscribe to topic: {MQTT_TOPIC}. {e}   ") 
#_______________________________________________________________________________________________________________________________#
            
        # ESP1_TOPIC_TEMPERATURE                *ESP -> Topic -> Server Script -> Topic -> Dashboard
        try:
            MQTT_TOPIC          = ESP1_TOPIC_TEMPERATURE
            MQTT_TOPIC_SEND     = RPI_ESP1_TOPIC_TEMPERATURE

            client.subscribe(MQTT_TOPIC,2)
            client.subscribe(MQTT_TOPIC_SEND,2)

            if rc == 0:
                connectionReturnCodes = mqtt_dictionary.mqtt_dictionary()
                print(f" <OK> [mqtt_topic_connect][roomTemp]   Subscribed to topics: {MQTT_TOPIC, MQTT_TOPIC_SEND}, rc: {rc} - {connectionReturnCodes[rc]}.   ")

                try:  
                    try:
                        with open("esp_received_msg.json", "r") as json_file_roomTemp:
                            data = json.load(json_file_roomTemp)
                            v_roomTemperatureValue = data["esp32/no.1/roomTemp"]
                            print(f" <OK> [mqtt_topic_connect][roomTemp]   Opened the JSON.     ")
                    except Exception as e:
                        print(f" <!> [mqtt_topic_connect][roomTemp]  Failed to open the JSON.  {e}   ")

                    client.publish(MQTT_TOPIC_SEND,v_roomTemperatureValue)

                    print(f" <OK> [mqtt_topic_connect][roomTemp][publish]  Published to topic: {MQTT_TOPIC_SEND}: {v_roomTemperatureValue}.    ")
                except:
                    connectionReturnCodes = mqtt_dictionary.mqtt_dictionary()
                    print(f" <!> [mqtt_topic_connect][roomTemp][publish]  Failed to publish to topic: {MQTT_TOPIC_SEND}.    ")

            else:
                connectionReturnCodes = mqtt_dictionary.mqtt_dictionary()
                print(f" <!> [mqtt_topic_connect][roomTemp]   Failed to subscribe to topics: {MQTT_TOPIC, MQTT_TOPIC_SEND}. {e}   ")

        except TimeoutError :
            print(f" <!> [mqtt_topic_connect][roomTemp] --> TimeoutError <!>")
            pass

        except BlockingIOError:
            print(f" <!> [mqtt_topic_connect][roomTemp] --> BlockingIOError  <!>")
            pass

        except None as e:
            print(f" <!> [mqtt_topic_connect][roomTemp] Passed on None type error <!>")
            pass
  
        except Exception as e:
            MQTT_TOPIC          = ESP1_TOPIC_TEMPERATURE
            MQTT_TOPIC_SEND     = RPI_ESP1_TOPIC_TEMPERATURE
            print(f" <!> [mqtt_topic_connect][roomTemp]   Failed to subscribe to topics: {MQTT_TOPIC, MQTT_TOPIC_SEND}. {e}   ") 
#_______________________________________________________________________________________________________________________________#
            
        # ESP1_TOPIC_AIR_HUMIDITY               *ESP -> Topic -> Server Script -> Topic -> Dashboard
        try:
            MQTT_TOPIC          = ESP1_TOPIC_AIR_HUMIDITY
            MQTT_TOPIC_SEND     = RPI_ESP1_TOPIC_AIR_HUMIDITY

            client.subscribe(MQTT_TOPIC,2)
            client.subscribe(MQTT_TOPIC_SEND,2)

            if rc == 0:
                connectionReturnCodes = mqtt_dictionary.mqtt_dictionary()
                print(f" <OK> [mqtt_topic_connect][airHum]   Subscribed to topic: {MQTT_TOPIC, MQTT_TOPIC_SEND}, rc: {rc} - {connectionReturnCodes[rc]}.     ")

                try:
                    try:
                        with open("esp_received_msg.json", "r") as json_file_airHum:
                            data_airHum = json.load(json_file_airHum)
                            v_airHumidityValue = data_airHum["esp32/no.1/airHum"]
                            print(f" <OK> [mqtt_topic_connect][airHum]   Opened the JSON.     ")
                    except Exception as e:
                        print(print(f" <!> [mqtt_topic_connect][airHum]  Failed to open the JSON.  {e}    "))

                    client.publish(MQTT_TOPIC_SEND,v_airHumidityValue)
                    print(f" <OK> [mqtt_topic_connect][airHum][publish]  Published to topic: {MQTT_TOPIC_SEND}: {v_airHumidityValue}.   ")
                except:
                    connectionReturnCodes = mqtt_dictionary.mqtt_dictionary()
                    print(f" <!> [mqtt_topic_connect][airHum][publish]  Failed to publish to topic: {MQTT_TOPIC_SEND}.     ")

            else:
                connectionReturnCodes = mqtt_dictionary.mqtt_dictionary()
                print(f" <!> [mqtt_topic_connect][airHum]   Failed to subscribe to topics: {MQTT_TOPIC, MQTT_TOPIC_SEND}.     ") 
        
        except TimeoutError :
            print(f" <!> [mqtt_topic_connect][airHum] --> TimeoutError <!>")
            pass

        except BlockingIOError:
            print(f" <!> [mqtt_topic_connect][airHum] --> BlockingIOError  <!>")
            pass

        except None as e:
            print(f" <!> [mqtt_topic_connect][airHum] Passed on None type error <!>")
            pass

        except Exception as e:
            MQTT_TOPIC          = ESP1_TOPIC_AIR_HUMIDITY
            MQTT_TOPIC_SEND     = RPI_ESP1_TOPIC_AIR_HUMIDITY
            print(f" <!> [mqtt_topic_connect][airHum]   Failed to subscribe to topics: {MQTT_TOPIC, MQTT_TOPIC_SEND}. {e}   ") 

#_______________________________________________________________________________________________________________________________#
            
        # # ESP1_TOPIC_LED_CONTROL              *ESP <- Topic <- Server Script <- Topic <- Dashboard
        # try:
        #     MQTT_TOPIC       = RPI_ESP1_TOPIC_LED_CONTROL
        #     MQTT_TOPIC_SEND  = ESP1_TOPIC_LED_CONTROL

        #     client.subscribe(MQTT_TOPIC,1)
        #     client.subscribe(MQTT_TOPIC_SEND,1)

        #     if rc == 0:
        #         connectionReturnCodes = mqtt_dictionary.mqtt_dictionary()
        #         print(f"[mqtt_topic_connect]   Subscribed to topic: {MQTT_TOPIC}, rc: {rc} - {connectionReturnCodes[rc]}.     <OK>")

        #         try:
        #                 try:
        #                     with open("esp_received_msg.json", "r") as json_file:
        #                         data = json.load(json_file)
        #                         v_toggleLED = data["RaspPi/esp32/no.1/outputRed"]
        #                         print(print(f"[mqtt_topic_connect]   Opened the JSON.     <OK>"))
        #                 except Exception as e:
        #                     print(print(f"[mqtt_topic_connect]  Failed to open the JSON.  {e}    <!>"))

        #                 client.publish(MQTT_TOPIC_SEND,v_toggleLED)
        #                 print(f"[mqtt_topic_connect][airHum]  Published to topic: {MQTT_TOPIC_SEND}.    <OK>")
        #         except:
        #                 connectionReturnCodes = mqtt_dictionary.mqtt_dictionary()
        #                 print(f"[mqtt_topic_connect][airHum]  Failed to publish to topic: {MQTT_TOPIC_SEND}.     <!>")
        
        #     else:
        #         connectionReturnCodes = mqtt_dictionary.mqtt_dictionary()
        #         print(f"[mqtt_topic_connect]   Failed to subscribe to topic: {MQTT_TOPIC}, rc: {rc} - {connectionReturnCodes[rc]}.     <!>") 
        # except Exception as e:
        #     MQTT_TOPIC = RPI_ESP1_TOPIC_LED_CONTROL
        #     print(f"[mqtt_topic_connect]   Failed to subscribe to topic: {MQTT_TOPIC}. {e}    <!>") 

#_______________________________________________________________________________________________________________________________#
                        
        # # ESP1_TOPIC_RESET                    *ESP <- Topic <- Server Script <- Topic <- Dashboard
        # try:
        #     MQTT_TOPIC       = RPI_ESP1_TOPIC_RESET
        #     MQTT_TOPIC_SEND  = ESP1_TOPIC_RESET                


        #     client.subscribe(MQTT_TOPIC,1)
        #     client.subscribe(MQTT_TOPIC_SEND,1)

        #     if rc == 0:
        #         connectionReturnCodes = mqtt_dictionary.mqtt_dictionary()
        #         print(f"[mqtt_topic_connect]   Subscribed to topic: {MQTT_TOPIC}, rc: {rc} - {connectionReturnCodes[rc]}.     <OK>")

        #         try:
        #                 try:
        #                     with open("esp_received_msg.json", "r") as json_file:
        #                         data = json.load(json_file)
        #                         v_toggleReset= data["RaspPi/esp32/no.1/Reset"]
        #                         print(print(f"[mqtt_topic_connect]   Opened the JSON.     <OK>"))
        #                 except Exception as e:
        #                     print(print(f"[mqtt_topic_connect]  Failed to open the JSON.  {e}    <!>"))

        #                 client.publish(MQTT_TOPIC_SEND,v_toggleReset)
        #                 print(f"[mqtt_topic_connect][airHum]  Published to topic: {MQTT_TOPIC_SEND}.    <OK>")
        #         except:
        #                 connectionReturnCodes = mqtt_dictionary.mqtt_dictionary()
        #                 print(f"[mqtt_topic_connect][airHum]  Failed to publish to topic: {MQTT_TOPIC_SEND}.     <!>")
        
        #     else:
        #         connectionReturnCodes = mqtt_dictionary.mqtt_dictionary()
        #         print(f"[mqtt_topic_connect]   Failed to subscribe to topic: {MQTT_TOPIC}, rc: {rc} - {connectionReturnCodes[rc]}.     <!>") 
        # except Exception as e:
        #     MQTT_TOPIC = RPI_ESP1_TOPIC_LED_CONTROL
        #     print(f"[mqtt_topic_connect]   Failed to subscribe to topic: {MQTT_TOPIC}. {e}    <!>") 

#_______________________________________________________________________________________________________________________________#
                     
        # ESP1_TOPIC_SYS_STATE                  *ESP -> Topic -> Server Script -> Topic -> Dashboard
        try:
            MQTT_TOPIC          = ESP1_TOPIC_SYS_STATE
            MQTT_TOPIC_SEND     = RPI_ESP1_TOPIC_SYS_STATE

            client.subscribe(MQTT_TOPIC,2)
            client.subscribe(MQTT_TOPIC_SEND,2)

            if rc == 0:
                connectionReturnCodes = mqtt_dictionary.mqtt_dictionary()
                print(f" <OK> [mqtt_topic_connect][sysState]   Subscribed to topics: {MQTT_TOPIC, MQTT_TOPIC_SEND}, rc: {rc} - {connectionReturnCodes[rc]}.    ")
        
                try:
                    try:    
                        with open("esp_received_msg.json","r") as json_file_sysState:
                            data_sysState = json.load(json_file_sysState)
                            v_sysStateValue = data_sysState["esp32/no.1/sysState"]
                            print(f" <OK> [mqtt_topic_connect][sysState]   Opened the JSON.    ")
                    except Exception as e:
                        print(print(f" <!> [mqtt_topic_connect][sysState]   Failed to open the JSON.  {e}   "))

                    client.publish(MQTT_TOPIC_SEND, v_sysStateValue)
                    print(f" <OK> [mqtt_topic_connect][sysState][publish]  Published to topic: {MQTT_TOPIC_SEND} :{v_sysStateValue}.   ")
                except:
                    connectionReturnCodes = mqtt_dictionary.mqtt_dictionary()
                    print(f" <!> [mqtt_topic_connect][sysState][publish]  Failed to publish to topic: {MQTT_TOPIC_SEND}.    ")
        
            else:
                connectionReturnCodes = mqtt_dictionary.mqtt_dictionary()
                print(f" <!> [mqtt_topic_connect][sysState]   Failed to subscribe to topics: {MQTT_TOPIC, MQTT_TOPIC_SEND}, rc: {rc} - {connectionReturnCodes[rc]}.    ")

        except TimeoutError :
            print(f" <!> [mqtt_topic_connect][sysState] --> TimeoutError <!>")
            pass

        except BlockingIOError:
            print(f" <!> [mqtt_topic_connect][sysState] --> BlockingIOError  <!>")
            pass

        except None as e:
            print(f" <!> [mqtt_topic_connect][sysState] Passed on None type error <!>")
            pass
 
        except Exception as e:
            MQTT_TOPIC          = ESP1_TOPIC_SYS_STATE
            MQTT_TOPIC_SEND     = RPI_ESP1_TOPIC_SYS_STATE
            print(f" <!> [mqtt_topic_connect][sysState]   Failed to subscribe to topics: {MQTT_TOPIC, MQTT_TOPIC_SEND}. {e}   ") 

#_______________________________________________________________________________________________________________________________#
            
        # ESP1_TOPIC_TEMP_CONTROL              *Dashboard -> Topic -> Server Script -> Topic -> ESP
        try:
            MQTT_TOPIC      = RPI_ESP1_TOPIC_TEMP_CONTROL
            MQTT_TOPIC_SEND = ESP1_TOPIC_TEMP_CONTROL

            client.subscribe(MQTT_TOPIC,2)
            client.subscribe(MQTT_TOPIC_SEND,2)

            if rc == 0:
                connectionReturnCodes = mqtt_dictionary.mqtt_dictionary()
                print(f" <OK> [mqtt_topic_connect][tempCtrl]   Subscribed to topics: {MQTT_TOPIC, MQTT_TOPIC_SEND}, rc: {rc} - {connectionReturnCodes[rc]}.    ")

                try:
                    try:
                        with open("esp_received_msg.json","r") as json_file_tempCtrl:
                            data_tempCtrl = json.load(json_file_tempCtrl)
                            v_tempCtrlValue = data_tempCtrl["RaspPi/esp32/no.1/temperatureControl"]
                            print(f" <OK> [mqtt_topic_connect][tempCtrl]   Opened the JSON.     ")
                    except Exception as e:
                        print(print(f" <!> [mqtt_topic_connect][tempCtrl]   Failed to open the JSON.  {e}   "))

                    client.publish(MQTT_TOPIC_SEND, v_tempCtrlValue)
                    print(f" <OK> [mqtt_topic_connect][tempCtrl][publish]  Published to topic: {MQTT_TOPIC_SEND} :{v_tempCtrlValue}.   ")

                except:
                    connectionReturnCodes = mqtt_dictionary.mqtt_dictionary()
                    print(f" <!> [mqtt_topic_connect][tempCtrl][publish]  Failed to publish to topic: {MQTT_TOPIC_SEND}.    ")

            else:
                connectionReturnCodes = mqtt_dictionary.mqtt_dictionary()
                print(f" <!> [mqtt_topic_connect][tempCtrl]   Failed to subscribe to topics: {MQTT_TOPIC, MQTT_TOPIC_SEND}, rc: {rc} - {connectionReturnCodes[rc]}.    ") 

        except TimeoutError :
            print(f" <!> [mqtt_topic_connect][tempCtrl] --> TimeoutError <!>")
            pass

        except BlockingIOError:
            print(f" <!> [mqtt_topic_connect][tempCtrl]--> BlockingIOError  <!>")
            pass

        except None as e:
            print(f" <!> [mqtt_topic_connect][tempCtrl] Passed on None type error <!>")
            pass

        except Exception as e:
            MQTT_TOPIC      = ESP1_TOPIC_TEMP_CONTROL
            MQTT_TOPIC_SEND = RPI_ESP1_TOPIC_TEMP_CONTROL
            print(f" <!> [mqtt_topic_connect][tempCtrl]   Failed to subscribe to topics: {MQTT_TOPIC, MQTT_TOPIC_SEND}. {e}  ") 
       

#_______________________________________________________________________________________________________________________________#
#________________________________________________________RETURN_AREA____________________________________________________________#

#_______________________________________________________________________________________________________________________________#
#________________________________________________________ DEBUG_AREA____________________________________________________________#