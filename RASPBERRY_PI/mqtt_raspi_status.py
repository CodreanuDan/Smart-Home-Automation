#_______________________________________________________________________________________________________________________________#
#_______________________________________________________DESCRIPTION:____________________________________________________________#
#
# --> Module that handles message sending of RaspberryPi status
#
#_______________________________________________________________________________________________________________________________#

#_______________________________________________________________________________________________________________________________#
#_____________________________________________________IMPORT_LIBRARIES__________________________________________________________#
from on_message_json_dump import on_message_json_dump  # Module responsable for JSON Dump of the values that came from topics, so that they can be accsesed with ease by other modules(mqtt_topic_subscribe).
import paho.mqtt.client as mqtt
from datetime import datetime
import time
import asyncio

#_______________________________________________________________________________________________________________________________#
#______________________________________________________IMPORT MODULES___________________________________________________________#
from get_wifi_info import get_wifi_info             # Class that handles the communication with the wifi API.
from get_outside_temp  import get_outside_temp      # Class that handles the communication with the weather API  

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


    def get_outside_temperature_status(self,client,MQTT_TOPIC_DASH,MQTT_TOPIC_ESP,location):
        
        try:
            outside_temp_message = None
            try:
                outside_temp_message = asyncio.run(get_outside_temp.get_weather(location)) # Call the method on the class
                jsonDumperSetLocation= on_message_json_dump()
                if outside_temp_message != None:
                    jsonDumperSetLocation.json_dump_outTemp(outside_temp_message)
                else:
                    print(f" <!> [mqtt_raspi_status] from [get_outside_temp]: Could not get the temperature, None Type:. {e}     ")
            except Exception as e:
                print(f" <OK> [mqtt_raspi_status] from [get_outside_temp]: Exception {e}   ")    

            print(f" <OK> [mqtt_raspi_status] from [get_outside_temp]: Outside Temperature: {outside_temp_message}°C   ")    

            client.publish(MQTT_TOPIC_DASH, outside_temp_message)
            client.publish(MQTT_TOPIC_ESP, outside_temp_message)
            print(f" <OK> [mqtt_raspi_status]   Topic: {MQTT_TOPIC_DASH}(Dash);    Published Outside Temperature: {outside_temp_message}°C     ")
            print(f" <OK> [mqtt_raspi_status]   Topic: {MQTT_TOPIC_ESP}(ESP);    Published Outside Temperature: {outside_temp_message}°C     ")
        except Exception as e:
            print(f" <!> [mqtt_raspi_status]   Topic: {MQTT_TOPIC_DASH}(Dash);    Could not send Outside Temperature: {outside_temp_message}°C. {e}      ")
            print(f" <!> [mqtt_raspi_status]   Topic: {MQTT_TOPIC_ESP}(ESP);    Could not send Outside Temperature: {outside_temp_message}°C. {e}      ")
            pass
        

#_______________________________________________________________________________________________________________________________#
#________________________________________________________RETURN_AREA____________________________________________________________#

#_______________________________________________________________________________________________________________________________#
#________________________________________________________ DEBUG_AREA____________________________________________________________#
if __name__ == "__main__":
    client = mqtt.Client()
    MQTT_TOPIC = "RaspPi/WiFiStatus"
    MQTT_TOPIC2 = "RaspPi/ScriptConToMQTT"
    MQTT_TOPIC3 = "RaspPi/CurrentTime"
    MQTT_TOPIC4 = "RaspPi/OutsideTemp"

    mqtt_raspi_status_instance = mqtt_raspi_status()

    mqtt_raspi_status_instance.script_conMQTT(client,MQTT_TOPIC2)
    mqtt_raspi_status_instance.wifi_status(client,MQTT_TOPIC)
    mqtt_raspi_status_instance.get_current_time(client,MQTT_TOPIC3)
    mqtt_raspi_status_instance.get_outside_temperature_status(client,MQTT_TOPIC4,location = "Ruginoasa")