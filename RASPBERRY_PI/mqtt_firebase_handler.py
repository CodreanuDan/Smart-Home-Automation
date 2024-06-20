#_______________________________________________________________________________________________________________________________#
#_______________________________________________________DESCRIPTION:____________________________________________________________#
#
# --> Module responsable for establishing connection to firbase and fetch the data for temp ctrl and json dump it.
#
#_______________________________________________________________________________________________________________________________#
#_____________________________________________________IMPORT_LIBRARIES__________________________________________________________#
import paho.mqtt.client as mqtt
from firebase_admin import credentials, db
import firebase_admin
import json 
#_______________________________________________________________________________________________________________________________#
#______________________________________________________IMPORT MODULES___________________________________________________________#
from on_message_json_dump import on_message_json_dump  # Module responsable for JSON Dump of the values that came from topics, so that they can be accsesed with ease by other modules(mqtt_topic_subscribe).

#_______________________________________________________________________________________________________________________________#
#_____________________________________________________GLOBAL_VARIABLES__________________________________________________________#
RPI_FIREBASE_TOPIC_TEMP_CONTROL = 'RaspPi/Firebase/temperatureControl'           # (Recives tempCtrl)     (Sends tempCtrl)

#_______________________________________________________________________________________________________________________________#
#_______________________________________________________MAIN_FUNCTION___________________________________________________________#
class mqtt_firebase_handler:
    
    def __init__(self, client):
        
        #------------Firebase-Connection----------------#
        self.service_account_path = '/home/dan/SmartHome/service_account_path.json' # Path to service account key
        self.cred = credentials.Certificate(self.service_account_path)# Initialize the app with a service account, granting admin privileges
        firebase_admin.initialize_app(self.cred, {
            'databaseURL' : 'https://smart-home-iot-codreanu-dan-default-rtdb.europe-west1.firebasedatabase.app/'  # project id
        })
        self.root = db.reference() # Reference to the root of your database
        self.temp_ctrl_ref = self.root.child('NRD/ESP/TempCtrl')  # Get a reference to the 'NRD/ESP/TempCtrl' node
        #----------------MQTT-Connection----------------#
        self.client = client
        self.MQTT_TOPIC_SEND = RPI_FIREBASE_TOPIC_TEMP_CONTROL
        client.subscribe(self.MQTT_TOPIC_SEND,2)


    def listener(self, event): # Define a listener for the 'TempCtrl' node and use event to fetch data
        
        #------------Firebase-Connection-and-data-retrieve-------------#
        print("TempCtrl data:")
        print(event.data)
        self.tempCtrl_data = str(event.data)
        #-----------------------------JSON-Dump-------------------------#
        on_message_json_dump.json_dump_tempCtrl_firebase(self, self.tempCtrl_data) # bind json_dump tot the listener event
        #----------------MQTT-Connection-and-publish-data---------------#
        try:
            client.publish(self.MQTT_TOPIC_SEND, self.tempCtrl_data)
            print(f" <OK> [mqtt_firbease_handler][tempCtrl][publish]  Published to topic: {self.MQTT_TOPIC_SEND} :{self.tempCtrl_data}")	 
        except Exception as e:
            print(f" <!> [mqtt_firbease_handler][tempCtrl][publish]  Error publishing to topic: {self.MQTT_TOPIC_SEND} :{self.tempCtrl_data} : {e}")
            pass

    def mqtt_fetch_data_from_firebase(self):
        self.temp_ctrl_ref.listen(self.listener)  # Add the listener to the 'TempCtrl' node
    
    
#_______________________________________________________________________________________________________________________________#
#________________________________________________________ DEBUG_AREA____________________________________________________________#
if __name__ == "__main__":
    client = mqtt.Client()  # Create an instance of the mqtt.Client
    firebase_handler = mqtt_firebase_handler(client) # Create an instance of the class mqtt_firebase_handler
    firebase_handler.mqtt_fetch_data_from_firebase() # Call the method on the instance of the class
    