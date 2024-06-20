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
# except TimeoutError :
#     print(f" <!> [mqtt_topic_connect][outsideTemp] --> TimeoutError <!>")
#     pass
#
# except BlockingIOError:
#     print(f" <!> [mqtt_topic_connect][outsideTemp]--> BlockingIOError  <!>")
#     pass
#
# except None as e:
#     print(f" <!> [mqtt_topic_connect][outsideTemp] Passed on None type error <!>")
#     pass
#
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
import asyncio


#_______________________________________________________________________________________________________________________________#
#______________________________________________________IMPORT MODULES___________________________________________________________#
from mqtt_dictionary   import mqtt_dictionary       # Dictionaty containing MQTT RC Codes
from mqtt_raspi_status import mqtt_raspi_status     # Class that contains functions that are retriving data from RaspPi
from mqtt_on_message   import mqtt_on_message       # Class that handles the reciving the messages and esp_received_msg.json
from get_outside_temp  import get_outside_temp

#_______________________________________________________________________________________________________________________________#
#_____________________________________________________GLOBAL_VARIABLES__________________________________________________________#

# --> TOPICS:

# a) Module 3 -RaspberryPi 4 TOPICS:

MQTT_TOPIC_CHECK_STATUS             = 'RaspPi/ScriptConToMQTT'                                      # (Checks status from RPI)     (Sends status to dashboard)
MQTT_TOPIC_WIFI                     = 'RaspPi/WiFiStatus'                                           # (Recives wifi status)         (Sends wifi status to dashboard)
MQTT_TOPIC_TIME                     = 'RaspPi/CurrentTime'                                          # (Recives time)                (Sends time to dashboard)

# b.1) Module 1 - ESP32/No.1 TOPICS:

ESP1_TOPIC_TEMPERATURE              = 'esp32/no.1/roomTemp'                                         # (Recives temp from esp)         (Sends temp to json)        
ESP1_TOPIC_AIR_HUMIDITY             = 'esp32/no.1/airHum'                                           # (Recives airHum from esp)       (Sends airHum to json)
ESP1_TOPIC_SYS_STATE                = 'esp32/no.1/sysState'                                         # (Recives sysState)     (Sends sysState)
ESP1_TOPIC_TEMP_CONTROL             = 'esp32/no.1/temperatureControl'                               # (Recives tempCtrl)     (Sends tempCtrl)
ESP1_TOPIC_OUTSIDE_TEMP_GET         = 'esp32/no.1/getOutsideTemp'                                   # (Recives temp from get_outside_temp function)  (ESP)
ESP1_TOPIC_AIR_PRESS                = 'esp32/no.1/airPressure'                                     # (Recives airPressure)  (Sends airPressure)                  
# ESP1_TOPIC_LED_CONTROL          = 'esp32/no.1/outputRed'
# ESP1_TOPIC_RESET                = 'esp32/no.1/Reset'
ESP1_TOPIC_IAQ                      = 'esp32/no.1/IAQ'                                              # (Recives IAQ)          (Sends IAQ)
ESP1_TOPIC_CO2                      = 'esp32/no.1/eCO2'                                              # (Recives CO2)          (Sends CO2)
ESP1_TOPIC_TVOC                     = 'esp32/no.1/TVOC'                                             # (Recives TVOC)         (Sends TVOC)
ESP1_TOPIC_ETOH                     = 'esp32/no.1/EtOH'                                             # (Recives ETOH)         (Sends ETOH)

# b.2) Module 1 - ESP32/No.1 TOPICS to NodeRed Dashboard via RaspPi Python Program:

RPI_ESP1_TOPIC_TEMPERATURE          = 'RaspPi/esp32/no.1/roomTemp'                                  # (Recives temp from json)         (Sends temp to dashboard)
RPI_ESP1_TOPIC_AIR_HUMIDITY         = 'RaspPi/esp32/no.1/airHum'                                    # (Recives airHum from json)       (Sends airHum to dashboard)
RPI_ESP1_TOPIC_SYS_STATE            = 'RaspPi/esp32/no.1/sysState'                                  # (Recives sysState from json)                   (Sends sysState to dashboard)                              
RPI_ESP1_TOPIC_TEMP_CONTROL         = 'RaspPi/esp32/no.1/temperatureControl'                        # (Recives tempCtrl from dashboard)              (Sends tempCtrl to json)     
RPI_ESP1_TOPIC_SET_LOCATION         = 'RaspPi/esp32/no.1/setLocation'                               # (Recives location from dashboard)              (Sends location to get_outside_temp function)           
RPI_ESP1_TOPIC_OUTSIDE_TEMP_GET     = 'RaspPi/esp32/no.1/getOutsideTemp'                            # (Recives temp from get_outside_temp function)  (Sends temp to dashboard and ESP)                       
RPI_ESP1_TOPIC_AIR_PRESS            = 'RaspPi/esp32/no.1/airPressure'                               # (Recives airPressure from json)                (Sends airPressure to dashboard) 
# RPI_ESP1_TOPIC_LED_CONTROL      = 'RaspPi/esp32/no.1/outputRed'
# RPI_ESP1_TOPIC_RESET            = 'RaspPi/esp32/no.1/Reset'
RPI_ESP1_TOPIC_IAQ                  = 'RaspPi/esp32/no.1/IAQ'                                       # (Recives IAQ from json)                        (Sends IAQ to dashboard)
RPI_ESP1_TOPIC_CO2                  = 'RaspPi/esp32/no.1/eCO2'                                       # (Recives CO2 from json)                        (Sends CO2 to dashboard)
RPI_ESP1_TOPIC_TVOC                 = 'RaspPi/esp32/no.1/TVOC'                                      # (Recives TVOC from json)                       (Sends TVOC to dashboard)
RPI_ESP1_TOPIC_ETOH                 = 'RaspPi/esp32/no.1/EtOH'                                      # (Recives ETOH from json)                       (Sends ETOH to dashboard)



# --> GLOBAL VARIABLES:

#_______________________________________________________________________________________________________________________________#
#_______________________________________________________MAIN_FUNCTION___________________________________________________________#

class mqtt_topic_subscribe():                   # Class that realises the connection to MQTT Topics. Inside each block are implemented functionalities based for each topic apart.
    
    """

    Function topic_subscribe(self,client,rc): is a function that subscribe to specific MQTT topics and then 
    takes the specific steps to read the data from the json and make connection between topics from the ESP 
    or Node Red Dasboard or directly from the server script to either ESP or Node Red Dashboard
                          Server Script -> Topic -> Dashboard
                          ESP -> Topic -> Server Script -> Topic -> Dashboard
                          Dashboard -> Topic -> Server Script -> Topic -> ESP

    """

    def topic_subscribe(self,client,rc):        # The self parameter represents the instance of the class.

        mqtt_topic_subscribe.mqtt_topicSub_check_status(self,client,rc)       # Subscribes to the MQTT_TOPIC_CHECK_STATUS
        mqtt_topic_subscribe.mqtt_topicSub_wifi_status(self,client,rc)        # Subscribes to the MQTT_TOPIC_WIFI
        mqtt_topic_subscribe.mqtt_topicSub_current_time(self,client,rc)       # Subscribes to the MQTT_TOPIC_TIME
        mqtt_topic_subscribe.mqtt_topicSub_room_temp(self,client,rc)          # Subscribes to the ESP1_TOPIC_TEMPERATURE
        mqtt_topic_subscribe.mqtt_topicSub_air_humidity(self,client,rc)       # Subscribes to the ESP1_TOPIC_AIR_HUMIDITY
        mqtt_topic_subscribe.mqtt_topicSub_sys_state(self,client,rc)          # Subscribes to the ESP1_TOPIC_SYS_STATE
        mqtt_topic_subscribe.mqtt_topicSub_temp_control(self,client,rc)       # Subscribes to the ESP1_TOPIC_TEMP_CONTROL       
        mqtt_topic_subscribe.mqtt_topicSub_outside_temp(self,client,rc)       # Subscribes to the RPI_ESP1_TOPIC_OUTSIDE_TEMP_GET
        mqtt_topic_subscribe.mqtt_topicSub_air_Pressure(self,client,rc)       # Subscribes to the ESP1_TOPIC_AIR_PRESS
        mqtt_topic_subscribe.mqtt_topicSub_IAQ(self,client,rc)                # Subscribes to the ESP1_TOPIC_IAQ
        mqtt_topic_subscribe.mqtt_topicSub_CO2(self,client,rc)                # Subscribes to the ESP1_TOPIC_CO2
        mqtt_topic_subscribe.mqtt_topicSub_TVOC(self,client,rc)               # Subscribes to the ESP1_TOPIC_TVOC
        mqtt_topic_subscribe.mqtt_topicSub_ETOH(self,client,rc)               # Subscribes to the ESP1_TOPIC_ETOH

#####################################################################################################################################
#|                                                    RASPI RELATED TOPICS                                                         |#
#####################################################################################################################################
    '''
            Server Script -> Topic -> Dashboard

    '''

#_______________________________________________________________________________________________________________________________#

    def mqtt_topicSub_check_status(self,client,rc):        # The self parameter represents the instance of the class.
        
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
            print(f" <!> [mqtt_topic_connect][rpiStatus]   Failed to subscribe to topic: {MQTT_TOPIC}. {e}   ") 

#_______________________________________________________________________________________________________________________________#

    def mqtt_topicSub_wifi_status(self,client,rc):         # The self parameter represents the instance of the class.
            
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

    def mqtt_topicSub_current_time(self,client,rc):        # The self parameter represents the instance of the class.
            
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
       
        #####################################################################################################################################
        #|                                                    ESP32 RELATED TOPICS                                                         |#
        #####################################################################################################################################
        '''
                ESP -> Topic -> Server Script -> Topic -> Dashboard
        '''

#_______________________________________________________________________________________________________________________________#

    def mqtt_topicSub_room_temp(self,client,rc):           # The self parameter represents the instance of the class.
            
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

    def mqtt_topicSub_air_humidity(self,client,rc):        # The self parameter represents the instance of the class.
            
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

    def mqtt_topicSub_sys_state(self,client,rc):           # The self parameter represents the instance of the class.  
                     
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

    def mqtt_topicSub_air_Pressure(self,client,rc):        # The self parameter represents the instance of the class.  
                        
            # ESP1_TOPIC_AIR_PRESS                 *ESP -> Topic -> Server Script -> Topic -> Dashboard
            try:
                MQTT_TOPIC          = ESP1_TOPIC_AIR_PRESS 
                MQTT_TOPIC_SEND     = RPI_ESP1_TOPIC_AIR_PRESS 

                client.subscribe(MQTT_TOPIC,2)
                client.subscribe(MQTT_TOPIC_SEND,2)

                if rc == 0:
                    connectionReturnCodes = mqtt_dictionary.mqtt_dictionary()
                    print(f" <OK> [mqtt_topic_connect][airPressure]   Subscribed to topics: {MQTT_TOPIC, MQTT_TOPIC_SEND}, rc: {rc} - {connectionReturnCodes[rc]}.    ")
            
                    try:
                        try:    
                            with open("esp_received_msg.json","r") as json_file_airPressure:
                                data_airPressure = json.load(json_file_airPressure)
                                v_airPressureValue = data_airPressure["esp32/no.1/airPressure"]
                                print(f" <OK> [mqtt_topic_connect][airPressure]   Opened the JSON.    ")
                        except Exception as e:
                            print(print(f" <!> [mqtt_topic_connect][airPressure]   Failed to open the JSON.  {e}   "))

                        client.publish(MQTT_TOPIC_SEND, v_airPressureValue)
                        print(f" <OK> [mqtt_topic_connect][airPressure][publish]  Published to topic: {MQTT_TOPIC_SEND} :{v_airPressureValue}.   ")
                    except:
                        connectionReturnCodes = mqtt_dictionary.mqtt_dictionary()
                        print(f" <!> [mqtt_topic_connect][airPressure][publish]  Failed to publish to topic: {MQTT_TOPIC_SEND}.    ")
            
                else:
                    connectionReturnCodes = mqtt_dictionary.mqtt_dictionary()
                    print(f" <!> [mqtt_topic_connect][airPressure]   Failed to subscribe to topics: {MQTT_TOPIC, MQTT_TOPIC_SEND}, rc: {rc} - {connectionReturnCodes[rc]}.    ")

            except TimeoutError :
                print(f" <!> [mqtt_topic_connect][airPressure] --> TimeoutError <!>")
                pass

            except BlockingIOError:
                print(f" <!> [mqtt_topic_connect][airPressure] --> BlockingIOError  <!>")
                pass

            except None as e:
                print(f" <!> [mqtt_topic_connect][airPressure] Passed on None type error <!>")
                pass
    
            except Exception as e:
                MQTT_TOPIC          = ESP1_TOPIC_AIR_PRESS
                MQTT_TOPIC_SEND     = RPI_ESP1_TOPIC_AIR_PRESS
                print(f" <!> [mqtt_topic_connect][airPressure]   Failed to subscribe to topics: {MQTT_TOPIC, MQTT_TOPIC_SEND}. {e}   ") 
                
#_______________________________________________________________________________________________________________________________#

    def mqtt_topicSub_IAQ(self,client,rc):                 # The self parameter represents the instance of the class.
        
        # ESP1_TOPIC_IAQ                        *ESP -> Topic -> Server Script -> Topic -> Dashboard
        try:
            MQTT_TOPIC          = ESP1_TOPIC_IAQ
            MQTT_TOPIC_SEND     = RPI_ESP1_TOPIC_IAQ

            client.subscribe(MQTT_TOPIC,2)
            client.subscribe(MQTT_TOPIC_SEND,2)

            if rc == 0:
                connectionReturnCodes = mqtt_dictionary.mqtt_dictionary()
                print(f" <OK> [mqtt_topic_connect][IAQ]   Subscribed to topics: {MQTT_TOPIC, MQTT_TOPIC_SEND}, rc: {rc} - {connectionReturnCodes[rc]}.    ")
        
                try:
                    try:    
                        with open("esp_received_msg.json","r") as json_file_IAQ:
                            data_IAQ = json.load(json_file_IAQ)
                            v_IAQValue = data_IAQ["esp32/no.1/IAQ"]
                            print(f" <OK> [mqtt_topic_connect][IAQ]   Opened the JSON.    ")
                    except Exception as e:
                        print(print(f" <!> [mqtt_topic_connect][IAQ]   Failed to open the JSON.  {e}   "))

                    client.publish(MQTT_TOPIC_SEND, v_IAQValue)
                    print(f" <OK> [mqtt_topic_connect][IAQ][publish]  Published to topic: {MQTT_TOPIC_SEND} :{v_IAQValue}.   ")
                except:
                    connectionReturnCodes = mqtt_dictionary.mqtt_dictionary()
                    print(f" <!> [mqtt_topic_connect][IAQ][publish]  Failed to publish to topic: {MQTT_TOPIC_SEND}.    ")
        
            else:
                connectionReturnCodes = mqtt_dictionary.mqtt_dictionary()
                print(f" <!> [mqtt_topic_connect][IAQ]   Failed to subscribe to topics: {MQTT_TOPIC, MQTT_TOPIC_SEND}, rc: {rc} - {connectionReturnCodes[rc]}.    ")
        
        except TimeoutError :
            print(f" <!> [mqtt_topic_connect][IAQ] --> TimeoutError <!>")
            pass
        
        except BlockingIOError:
            print(f" <!> [mqtt_topic_connect][IAQ] --> BlockingIOError  <!>")
            pass
        
        except None as e:
            print(f" <!> [mqtt_topic_connect][IAQ] Passed on None type error <!>")
            pass
        
        except Exception as e:
            MQTT_TOPIC          = ESP1_TOPIC_IAQ
            MQTT_TOPIC_SEND     = RPI_ESP1_TOPIC_IAQ
            print(f" <!> [mqtt_topic_connect][IAQ]   Failed to subscribe to topics: {MQTT_TOPIC, MQTT_TOPIC_SEND}. {e}   ")
                    
#_______________________________________________________________________________________________________________________________#

    def mqtt_topicSub_CO2(self,client,rc):                 # The self parameter represents the instance of the class.
        
        # ESP1_TOPIC_CO2                        *ESP -> Topic -> Server Script -> Topic -> Dashboard
        try:
            MQTT_TOPIC          = ESP1_TOPIC_CO2
            MQTT_TOPIC_SEND     = RPI_ESP1_TOPIC_CO2
            
            client.subscribe(MQTT_TOPIC,2)
            client.subscribe(MQTT_TOPIC_SEND,2)
            
            if rc == 0:
                connectionReturnCodes = mqtt_dictionary.mqtt_dictionary()
                print(f" <OK> [mqtt_topic_connect][CO2]   Subscribed to topics: {MQTT_TOPIC, MQTT_TOPIC_SEND}, rc: {rc} - {connectionReturnCodes[rc]}.    ")
        
                try:
                    try:    
                        with open("esp_received_msg.json","r") as json_file_CO2:
                            data_CO2 = json.load(json_file_CO2)
                            v_CO2Value = data_CO2["esp32/no.1/eCO2"]
                            print(f" <OK> [mqtt_topic_connect][CO2]   Opened the JSON.    ")
                    except Exception as e:
                        print(print(f" <!> [mqtt_topic_connect][CO2]   Failed to open the JSON.  {e}   "))

                    client.publish(MQTT_TOPIC_SEND, v_CO2Value)
                    print(f" <OK> [mqtt_topic_connect][CO2][publish]  Published to topic: {MQTT_TOPIC_SEND} :{v_CO2Value}.   ")
                except:
                    connectionReturnCodes = mqtt_dictionary.mqtt_dictionary()
                    print(f" <!> [mqtt_topic_connect][CO2][publish]  Failed to publish to topic: {MQTT_TOPIC_SEND}.    ")
                    
            else:
                connectionReturnCodes = mqtt_dictionary.mqtt_dictionary()
                print(f" <!> [mqtt_topic_connect][CO2]   Failed to subscribe to topics: {MQTT_TOPIC, MQTT_TOPIC_SEND}, rc: {rc} - {connectionReturnCodes[rc]}.    ")
                
        except TimeoutError :
            print(f" <!> [mqtt_topic_connect][CO2] --> TimeoutError <!>")
            pass
        
        except BlockingIOError:
            print(f" <!> [mqtt_topic_connect][CO2] --> BlockingIOError  <!>")
            pass
        
        except None as e:
            print(f" <!> [mqtt_topic_connect][CO2] Passed on None type error <!>")
            pass
        
        except Exception as e:
            MQTT_TOPIC          = ESP1_TOPIC_CO2
            MQTT_TOPIC_SEND     = RPI_ESP1_TOPIC_CO2
            print(f" <!> [mqtt_topic_connect][CO2]   Failed to subscribe to topics: {MQTT_TOPIC, MQTT_TOPIC_SEND}. {e}   ")
                     
#_______________________________________________________________________________________________________________________________#

    def mqtt_topicSub_TVOC(self,client,rc):                # The self parameter represents the instance of the class.
        
        # ESP1_TOPIC_TVOC                       *ESP -> Topic -> Server Script -> Topic -> Dashboard
        try:
            MQTT_TOPIC          = ESP1_TOPIC_TVOC
            MQTT_TOPIC_SEND     = RPI_ESP1_TOPIC_TVOC
            
            client.subscribe(MQTT_TOPIC,2)
            client.subscribe(MQTT_TOPIC_SEND,2)
            
            if rc == 0:
                connectionReturnCodes = mqtt_dictionary.mqtt_dictionary()
                print(f" <OK> [mqtt_topic_connect][TVOC]   Subscribed to topics: {MQTT_TOPIC, MQTT_TOPIC_SEND}, rc: {rc} - {connectionReturnCodes[rc]}.    ")
        
                try:
                    try:    
                        with open("esp_received_msg.json","r") as json_file_TVOC:
                            data_TVOC = json.load(json_file_TVOC)
                            v_TVOCValue = data_TVOC["esp32/no.1/TVOC"]
                            print(f" <OK> [mqtt_topic_connect][TVOC]   Opened the JSON.    ")
                    except Exception as e:
                        print(print(f" <!> [mqtt_topic_connect][TVOC]   Failed to open the JSON.  {e}   "))

                    client.publish(MQTT_TOPIC_SEND, v_TVOCValue)
                    print(f" <OK> [mqtt_topic_connect][TVOC][publish]  Published to topic: {MQTT_TOPIC_SEND} :{v_TVOCValue}.   ")
                except:
                    connectionReturnCodes = mqtt_dictionary.mqtt_dictionary()
                    print(f" <!> [mqtt_topic_connect][TVOC][publish]  Failed to publish to topic: {MQTT_TOPIC_SEND}.    ")
                    
            else:
                connectionReturnCodes = mqtt_dictionary.mqtt_dictionary()
                print(f" <!> [mqtt_topic_connect][TVOC]   Failed to subscribe to topics: {MQTT_TOPIC, MQTT_TOPIC_SEND}, rc: {rc} - {connectionReturnCodes[rc]}.    ")
                
        except TimeoutError :
            print(f" <!> [mqtt_topic_connect][TVOC] --> TimeoutError <!>")
            pass
        
        except BlockingIOError:
            print(f" <!> [mqtt_topic_connect][TVOC] --> BlockingIOError  <!>")
            pass
        
        except None as e:
            print(f" <!> [mqtt_topic_connect][TVOC] Passed on None type error <!>")
            pass
        
        except Exception as e:
            MQTT_TOPIC          = ESP1_TOPIC_TVOC
            MQTT_TOPIC_SEND     = RPI_ESP1_TOPIC_TVOC
            print(f" <!> [mqtt_topic_connect][TVOC]   Failed to subscribe to topics: {MQTT_TOPIC, MQTT_TOPIC_SEND}. {e}   ")
                      
#_______________________________________________________________________________________________________________________________#

    def mqtt_topicSub_ETOH(self,client,rc):                # The self parameter represents the instance of the class.
        
        # ESP1_TOPIC_ETOH                       *ESP -> Topic -> Server Script -> Topic -> Dashboard
        try:
            MQTT_TOPIC          = ESP1_TOPIC_ETOH
            MQTT_TOPIC_SEND     = RPI_ESP1_TOPIC_ETOH
            
            client.subscribe(MQTT_TOPIC,2)
            client.subscribe(MQTT_TOPIC_SEND,2)
            
            if rc == 0:
                connectionReturnCodes = mqtt_dictionary.mqtt_dictionary()
                print(f" <OK> [mqtt_topic_connect][EtOH]   Subscribed to topics: {MQTT_TOPIC, MQTT_TOPIC_SEND}, rc: {rc} - {connectionReturnCodes[rc]}.    ")
        
                try:
                    try:    
                        with open("esp_received_msg.json","r") as json_file_ETOH:
                            data_ETOH = json.load(json_file_ETOH)
                            v_ETOHValue = data_ETOH["esp32/no.1/EtOH"]
                            print(f" <OK> [mqtt_topic_connect][EtOH]   Opened the JSON.    ")
                    except Exception as e:
                        print(print(f" <!> [mqtt_topic_connect][EtOH]   Failed to open the JSON.  {e}   "))

                    client.publish(MQTT_TOPIC_SEND, v_ETOHValue)
                    print(f" <OK> [mqtt_topic_connect][EtOH][publish]  Published to topic: {MQTT_TOPIC_SEND} :{v_ETOHValue}.   ")
                except:
                    connectionReturnCodes = mqtt_dictionary.mqtt_dictionary()
                    print(f" <!> [mqtt_topic_connect][EtOH][publish]  Failed to publish to topic: {MQTT_TOPIC_SEND}.    ")
                    
            else:
                connectionReturnCodes = mqtt_dictionary.mqtt_dictionary()
                print(f" <!> [mqtt_topic_connect][EtOH]   Failed to subscribe to topics: {MQTT_TOPIC, MQTT_TOPIC_SEND}, rc: {rc} - {connectionReturnCodes[rc]}.    ")
                
        except TimeoutError :
            print(f" <!> [mqtt_topic_connect][EtOH] --> TimeoutError <!>")
            pass
        
        except BlockingIOError:
            print(f" <!> [mqtt_topic_connect][EtOH] --> BlockingIOError  <!>")
            pass
        
        except None as e:
            print(f" <!> [mqtt_topic_connect][EtOH] Passed on None type error <!>")
            pass
        
        except Exception as e:
            MQTT_TOPIC          = ESP1_TOPIC_ETOH
            MQTT_TOPIC_SEND     = RPI_ESP1_TOPIC_ETOH
            print(f" <!> [mqtt_topic_connect][EtOH]   Failed to subscribe to topics: {MQTT_TOPIC, MQTT_TOPIC_SEND}. {e}   ")
            

#####################################################################################################################################
#|                                                DASHBOARD RELATED TOPICS                                                         |#
#####################################################################################################################################
    '''
            Dashboard -> Topic -> Server Script -> Topic -> ESP
    '''
#_______________________________________________________________________________________________________________________________#
            
        # ESP1_TOPIC_LED_CONTROL              *ESP <- Topic <- Server Script <- Topic <- Dashboard

    # def mqtt_topicSub_led_control(self,client,rc):        # The self parameter represents the instance of the class.

    #     try:
    #         MQTT_TOPIC       = RPI_ESP1_TOPIC_LED_CONTROL
    #         MQTT_TOPIC_SEND  = ESP1_TOPIC_LED_CONTROL

    #         client.subscribe(MQTT_TOPIC,1)
    #         client.subscribe(MQTT_TOPIC_SEND,1)

    #         if rc == 0:
    #             connectionReturnCodes = mqtt_dictionary.mqtt_dictionary()
    #             print(f"[mqtt_topic_connect]   Subscribed to topic: {MQTT_TOPIC}, rc: {rc} - {connectionReturnCodes[rc]}.     <OK>")

    #             try:
    #                     try:
    #                         with open("esp_received_msg.json", "r") as json_file:
    #                             data = json.load(json_file)
    #                             v_toggleLED = data["RaspPi/esp32/no.1/outputRed"]
    #                             print(print(f"[mqtt_topic_connect]   Opened the JSON.     <OK>"))
    #                     except Exception as e:
    #                         print(print(f"[mqtt_topic_connect]  Failed to open the JSON.  {e}    <!>"))

    #                     client.publish(MQTT_TOPIC_SEND,v_toggleLED)
    #                     print(f"[mqtt_topic_connect][airHum]  Published to topic: {MQTT_TOPIC_SEND}.    <OK>")
    #             except:
    #                     connectionReturnCodes = mqtt_dictionary.mqtt_dictionary()
    #                     print(f"[mqtt_topic_connect][airHum]  Failed to publish to topic: {MQTT_TOPIC_SEND}.     <!>")
        
    #         else:
    #             connectionReturnCodes = mqtt_dictionary.mqtt_dictionary()
    #             print(f"[mqtt_topic_connect]   Failed to subscribe to topic: {MQTT_TOPIC}, rc: {rc} - {connectionReturnCodes[rc]}.     <!>") 
    #     except Exception as e:
    #         MQTT_TOPIC = RPI_ESP1_TOPIC_LED_CONTROL
    #         print(f"[mqtt_topic_connect]   Failed to subscribe to topic: {MQTT_TOPIC}. {e}    <!>") 

#_______________________________________________________________________________________________________________________________#
                        
        # ESP1_TOPIC_RESET                    *ESP <- Topic <- Server Script <- Topic <- Dashboard

    # def mqtt_topicSub_reset(self,client,rc):              # The self parameter represents the instance of the class.

    #     try:
    #         MQTT_TOPIC       = RPI_ESP1_TOPIC_RESET
    #         MQTT_TOPIC_SEND  = ESP1_TOPIC_RESET                


    #         client.subscribe(MQTT_TOPIC,1)
    #         client.subscribe(MQTT_TOPIC_SEND,1)

    #         if rc == 0:
    #             connectionReturnCodes = mqtt_dictionary.mqtt_dictionary()
    #             print(f"[mqtt_topic_connect]   Subscribed to topic: {MQTT_TOPIC}, rc: {rc} - {connectionReturnCodes[rc]}.     <OK>")

    #             try:
    #                     try:
    #                         with open("esp_received_msg.json", "r") as json_file:
    #                             data = json.load(json_file)
    #                             v_toggleReset= data["RaspPi/esp32/no.1/Reset"]
    #                             print(print(f"[mqtt_topic_connect]   Opened the JSON.     <OK>"))
    #                     except Exception as e:
    #                         print(print(f"[mqtt_topic_connect]  Failed to open the JSON.  {e}    <!>"))

    #                     client.publish(MQTT_TOPIC_SEND,v_toggleReset)
    #                     print(f"[mqtt_topic_connect][airHum]  Published to topic: {MQTT_TOPIC_SEND}.    <OK>")
    #             except:
    #                     connectionReturnCodes = mqtt_dictionary.mqtt_dictionary()
    #                     print(f"[mqtt_topic_connect][airHum]  Failed to publish to topic: {MQTT_TOPIC_SEND}.     <!>")
        
    #         else:
    #             connectionReturnCodes = mqtt_dictionary.mqtt_dictionary()
    #             print(f"[mqtt_topic_connect]   Failed to subscribe to topic: {MQTT_TOPIC}, rc: {rc} - {connectionReturnCodes[rc]}.     <!>") 
    #     except Exception as e:
    #         MQTT_TOPIC = RPI_ESP1_TOPIC_LED_CONTROL
    #         print(f"[mqtt_topic_connect]   Failed to subscribe to topic: {MQTT_TOPIC}. {e}    <!>") 
              
#_______________________________________________________________________________________________________________________________#

    def mqtt_topicSub_temp_control(self,client,rc):         # The self parameter represents the instance of the class.
            
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
        # RPI_ESP1_TOPIC_OUTSIDE_TEMP         *Dashboard -> Topic -> Server Script -> Topic -> ESP
        #                                    (Input location)       (Looks for temp)      (Controls temp)

    def mqtt_topicSub_outside_temp(self,client,rc):         # The self parameter represents the instance of the class.

        try:

            MQTT_TOPIC_SET = RPI_ESP1_TOPIC_SET_LOCATION
            MQTT_TOPIC_GET = RPI_ESP1_TOPIC_OUTSIDE_TEMP_GET
            MQTT_TOPIC_SEND = ESP1_TOPIC_OUTSIDE_TEMP_GET

            client.subscribe(MQTT_TOPIC_SET,2)
            client.subscribe(MQTT_TOPIC_GET,2)
            client.subscribe(MQTT_TOPIC_SEND,2)

            if rc == 0:

                connectionReturnCodes = mqtt_dictionary.mqtt_dictionary()
                print(f" <OK> [mqtt_topic_connect][outsideTemp]   Subscribed to topic: {MQTT_TOPIC_SET, MQTT_TOPIC_GET}, rc: {rc} - {connectionReturnCodes[rc]}.     ")


                try:    
                    with open("esp_received_msg.json","r") as json_file_setLocation:
                        data_setLocation = json.load(json_file_setLocation)
                        location = data_setLocation["RaspPi/esp32/no.1/setLocation"]
                        print(f" <OK> [mqtt_topic_connect][outsideTemp]   Opened the JSON.    ")
                except Exception as e:
                        print(print(f" <!> [mqtt_topic_connect][outsideTemp]   Failed to open the JSON.  {e}   "))

                if location != None:                                                                                                  # If the location is not None
                    handle_outiside_temp = mqtt_raspi_status()                                                                        # Create an instance 
                    handle_outiside_temp.get_outside_temperature_status(client,MQTT_TOPIC_GET,MQTT_TOPIC_SEND,location)               # Call the method on the instance
                else:
                    print(f"[mqtt_topic_connect][outsideTemp][DEBUG_MSG]: PASSED UPON LOCATION CHANGE & GET OUT TEMP, LOCATION IT`S NONE.")
                    pass

            else:
                connectionReturnCodes = mqtt_dictionary.mqtt_dictionary()
                print(f" <!> [mqtt_topic_connect][outsideTemp][publish]   Failed to subscribe to topic: {MQTT_TOPIC_SET, MQTT_TOPIC_GET, MQTT_TOPIC_SEND}, rc: {rc} - {connectionReturnCodes[rc]}.    ") 

        except TimeoutError :
            print(f" <!> [mqtt_topic_connect][outsideTemp] --> TimeoutError <!>")
            pass

        except BlockingIOError:
            print(f" <!> [mqtt_topic_connect][outsideTemp]--> BlockingIOError  <!>")
            pass

        except None as e:
            print(f" <!> [mqtt_topic_connect][outsideTemp] Passed on None type error <!>")
            pass

        except Exception as e:
            MQTT_TOPIC      = ESP1_TOPIC_TEMP_CONTROL
            MQTT_TOPIC_SEND = RPI_ESP1_TOPIC_TEMP_CONTROL
            print(f" <!> [mqtt_topic_connect][outsideTemp]   Failed to subscribe to topics: {MQTT_TOPIC, MQTT_TOPIC_SEND}. {e}  ") 

#_______________________________________________________________________________________________________________________________#
#________________________________________________________RETURN_AREA____________________________________________________________#

#_______________________________________________________________________________________________________________________________#
#________________________________________________________ DEBUG_AREA____________________________________________________________#
if __name__ == "__main__":      # Main function that is executed when the script is run
    pass
#_______________________________________________________________________________________________________________________________#
