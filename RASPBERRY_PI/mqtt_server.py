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
import threading


#_______________________________________________________________________________________________________________________________#
#______________________________________________________IMPORT MODULES___________________________________________________________#
from mqtt_on_connect import mqtt_on_connect             # Module that handles the callback function of the MQTT Client.
from mqtt_on_message import mqtt_on_message             # Module that handles the callback function of the MQTT Client for when a PUBLISH message is received from the server.)
from mqtt_dictionary import mqtt_dictionary             # Module that stores the most important info about MQTT Client status.     


#_______________________________________________________________________________________________________________________________#
#_____________________________________________________GLOBAL_VARIABLES__________________________________________________________#

#_______________________________________________________________________________________________________________________________#
#_______________________________________________________MAIN_FUNCTION___________________________________________________________#

class mqtt_connect():

    MQTT_BROKER = '169.254.175.150'
    print(" <OK> [mqtt_server][MAIN]  Connection to MQTT Broker ", MQTT_BROKER)

    mqtt.Client._thread = threading.current_thread()

    #####################################################################################################################################
    #|                                       ESTABLISH CONNECTION & START CLIENT LOOP FUNCTION                                         |#
    #####################################################################################################################################
    def mqtt_server_connection():

        try:           
            ######################################################################################################################
            #|                               FLAGS,VARS AND FUNCTIONS BINDED TO THE CLIENT                                      |#
            ######################################################################################################################

            client = mqtt.Client()                                   # Create an object to handle teh MQTT Client
            client.on_connect = mqtt_on_connect.on_connect           # What the client will do when connection is aknowleged (CONNAK)
            client.on_message = mqtt_on_message.on_message           # What the client will do when a message is received 

            client.connected_flag = False                            # Create flag to verify connection to MQTT Broker.

            ######################################################################################################################
            #|                                    ESTABLISH CONNECTION & START CLIENT LOOP                                      |#
            ######################################################################################################################
            while True:
                try:
                    MQTT_BROKER = '169.254.175.150'                                 # IP Adrress of the MQTT BROKER
                    MQTT_PORT = 1883                                                # PORT of the MQTT BROKER

                    try:  
                        client.loop_start()                                         # Start client loop: connect to MQTT Broker, proceed on binded functions: on_connect and on_message
                        '''
                            Kepp alive, 3rd argument of the function ensures that at least o package will be sent every 
                            x seconds to the MQTT Broker so the connection between it and the client is not lost.
                        '''
                        client.connect(MQTT_BROKER, MQTT_PORT, 5)                 
                    except Exception as e :
                        # client.reinitialise()
                        print(f" <!> [mqtt_server][MAIN]  Could not connect to MQTT Broker. {e}    <!>")

                    print(f" <OK> [mqtt_server][MAIN]  Connecting to MQTT Broker: {MQTT_BROKER}, MQTT PORT: {MQTT_PORT}")

                    while not client.connected_flag:                                # Wait in loop until connection succsessfull.
                        print(" <!> [mqtt_server][MAIN]   In wait loop ")
                        time.sleep(5)
    
                    print(f" <OK> [mqtt_server][MAIN]   In Main Loop ")

                    time.sleep(2.5)                                                 # Delay for client loop, best > 2 seconds.
                    print("_____________________________________\n")
                    print('\n')
                except Exception as e:
                    print(f" <!> [mqtt_server][MAIN]  Could not connect to MQTT Broker. {e}    <!>")
                    time.sleep(0.5)
                    client.reconnect()

        except KeyboardInterrupt:
                client.publish('RaspPi/WiFiStatus',"Disconnected")
                client.publish('RaspPi/ScriptConToMQTT',"Stopped")
                client.publish('RaspPi/CurrentTime','   -- : -- : --    ')
                print("\n[mqtt_raspi_status]   KeyboardInterrupt: Exiting the current time loop.")
                client.loop_stop()                                              # Stop loop 
                client.disconnect()                                             # Disconnect
                exit(0)
    
        except TimeoutError :
            print(f" <!> [mqtt_server][MAIN]  Could not connect to MQTT Broker --> TimeoutError <!>")
            pass
        
        # Used to correct the BlockingIOError that comes with bufsize : Bad file descriptor , socket error???
        except BlockingIOError:
            print(f" <!> [mqtt_server][MAIN]  Could not connect to MQTT Broker --> BlockingIOError  <!>")
            pass

        except None as e:
            print(f" <!> [mqtt_server][MAIN]  Passed on None type error <!>")
            pass

        except ConnectionError:
            print(f" <!> [mqtt_server][MAIN]  Could not connect to MQTT Broker --> BlockingIOError <!>")
            pass

        except ValueError:
            print(f" <!> [mqtt_server][MAIN]  Could not connect to MQTT Broker --> ValueError <!>")
            pass

        except OverflowError:
            print(f" <!> [mqtt_server][MAIN]  Could not connect to MQTT Broker --> OverflowError <!>")
            pass

        except Exception as e:
            print(f" <!> [mqtt_server][MAIN]  Could not connect to MQTT Broker. {e}    <!>")  
            # exit(1)


    #####################################################################################################################################
    #|                                   ESTABLISH CONNECTION & START CLIENT LOOP FUNCTION CALL                                        |#
    #####################################################################################################################################
    mqtt_server_connection()


   
#_______________________________________________________________________________________________________________________________#
#________________________________________________________RETURN_AREA____________________________________________________________#

#_______________________________________________________________________________________________________________________________#
#________________________________________________________ DEBUG_AREA____________________________________________________________#
    
