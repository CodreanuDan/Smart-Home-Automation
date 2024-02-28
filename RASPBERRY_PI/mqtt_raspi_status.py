#_______________________________________________________________________________________________________________________________#
#_______________________________________________________DESCRIPTION:____________________________________________________________#
#
# --> Module that handles message sending of RaspberryPi status
#
#_______________________________________________________________________________________________________________________________#

#_______________________________________________________________________________________________________________________________#
#_____________________________________________________IMPORT_LIBRARIES__________________________________________________________#
import paho.mqtt.client as mqtt
from datetime import datetime
import time

#_______________________________________________________________________________________________________________________________#
#______________________________________________________IMPORT MODULES___________________________________________________________#
from get_wifi_info import get_wifi_info

#_______________________________________________________________________________________________________________________________#
#_____________________________________________________GLOBAL_VARIABLES__________________________________________________________#

#_______________________________________________________________________________________________________________________________#
#_______________________________________________________MAIN_FUNCTION___________________________________________________________#

class mqtt_raspi_status():

    def script_conMQTT(self,client,MQTT_TOPIC):
        try:    
            status_message = "STATUS OK"
            client.publish(MQTT_TOPIC, status_message)
            print(f" <OK> [mqtt_raspi_status]   Topic: {MQTT_TOPIC};    Status message sent: {status_message}      ")
        except Exception as e:
            print(f" <!> [mqtt_raspi_status]   Topic: {MQTT_TOPIC};    Could not send status message sent: {status_message}. {e}     ")


    def wifi_status(self,client,MQTT_TOPIC):
        try:
            wifi_info_instance = get_wifi_info()                                 # Create an instance 
            wifi_info_message = wifi_info_instance.get_wifi_info()               # Call the method on the instance

            print(f" <OK> [mqtt_raspi_status] from [get_wifi_info]: WiFi Network Name: {wifi_info_message}   ")    

            client.publish(MQTT_TOPIC,wifi_info_message)
            print(f" <OK> [mqtt_raspi_status]   Topic: {MQTT_TOPIC};    WiFI status message sent: {wifi_info_message}     ")
        except Exception as e:
            print(f" <!> [mqtt_raspi_status]   Topic: {MQTT_TOPIC};    Could not send WiFI status message: {wifi_info_message}. {e}      ")


    def get_current_time(self,client,MQTT_TOPIC):
        try:
            current_time = datetime.now()
            formatted_time = str(current_time.strftime("%H:%M:%S"))

            print(f" <OK> [mqtt_raspi_status]     Current time: {formatted_time}    ")    

            client.publish(MQTT_TOPIC,formatted_time)
            print(f" <OK> [mqtt_raspi_status]   Topic: {MQTT_TOPIC};    current time: {formatted_time}      ")
        except Exception as e:
            print(f" <!> [mqtt_raspi_status]   Topic: {MQTT_TOPIC};    could not send current time: {formatted_time}. {e}    ")
            
        

#_______________________________________________________________________________________________________________________________#
#________________________________________________________RETURN_AREA____________________________________________________________#

#_______________________________________________________________________________________________________________________________#
#________________________________________________________ DEBUG_AREA____________________________________________________________#
