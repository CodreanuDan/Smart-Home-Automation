#_______________________________________________________________________________________________________________________________#
#_______________________________________________________DESCRIPTION:____________________________________________________________#
#
# --> Module responsable for JSON Dump of the values that came from topics, 
#     so that they can be accsesed with ease by other modules(mqtt_topic_subscribe).
#
# --> Used in module mqtt_on_message, mqtt_raspi_status.
#
#_______________________________________________________________________________________________________________________________#

#_______________________________________________________________________________________________________________________________#
#_____________________________________________________IMPORT_LIBRARIES__________________________________________________________#
import json
import time

#_______________________________________________________________________________________________________________________________#
#______________________________________________________IMPORT MODULES___________________________________________________________#

#_______________________________________________________________________________________________________________________________#
#_____________________________________________________GLOBAL_VARIABLES__________________________________________________________#

#_______________________________________________________________________________________________________________________________#
#_______________________________________________________MAIN_FUNCTION___________________________________________________________#

class on_message_json_dump():
    
    """
    JSON handlers for coming from FIREBASE to RaspPI : FIREBASE --> RaspPI Python program --> JSON
    Dumps received message from topic to json file : esp_received_msg.json

    """
    def json_dump_tempCtrl_firebase(self, tempCtrl_data):
        
        try:
            with open("esp_received_msg.json","r") as json_file_firebase_r:
                data_firebase = json.load(json_file_firebase_r)
                target_msg_firebase = data_firebase["RaspPi/esp32/no.1/temperatureControl"] = tempCtrl_data

                print(f" <OK> [mqtt_on_message][firebase]   Opened JSON for firebase!      ")
                
                try:
                    with open("esp_received_msg.json","w") as json_file_firebase_w:
                        if target_msg_firebase != None:
                            json.dump(data_firebase, json_file_firebase_w, indent = 4)
                            print(f" <OK> [mqtt_on_message][firebase]   Dumped into JSON for firebase!  {data_firebase}    ")
                        elif target_msg_firebase == None:
                            print(f" <!> [mqtt_on_message][firebase]   Received None Type message!  {data_firebase}    ")
                except Exception as e:
                    print(f" <!> [mqtt_on_message][firebase]   Could not dump JSON for firebase!  {e}    ")
        except Exception as e:
            print(f" <!> [mqtt_on_message][firebase]   Could not open JSON for firebase!  {e}    ")
    
    """
    JSON handlers for topics coming from ESP to RaspPI : ESP --> Topic --> RaspPI Python program --> JSON
    Dumps received message from topic to json file : esp_received_msg.json

    """
    
    # json dump for airTemp
    def json_dump_airTemp(self,msg):

        try:    
            with open("esp_received_msg.json", "r") as json_file_airTemp_r:
                data_airTemp = json.load(json_file_airTemp_r)
                target_msg_airTemp = data_airTemp["esp32/no.1/roomTemp"] = msg.payload.decode("utf-8")
                print(f" <OK> [mqtt_on_message][airTemp]   Opened JSON for airTemp!       ")

                try:
                    with open("esp_received_msg.json", "w") as json_file_airTemp_w:
                        if target_msg_airTemp != None:
                            json.dump(data_airTemp, json_file_airTemp_w, indent = 4)
                            print(f" <OK> [mqtt_on_message][airTemp]   Dumped into JSON for airTemp!  {data_airTemp}    ")
                        elif target_msg_airTemp == None:
                            print(f" <!> [mqtt_on_message][airTemp]   Received None Type message!  {data_airTemp}    ")
                except Exception as e:
                    print(f" <!> [mqtt_on_message][airTemp]   Could not dump JSON for airTemp!  {e}    ")

        except Exception as e:
                print(f" <!> [mqtt_on_message][airTemp]   Could not open JSON for airTemp!  {e}    ")

    # json dump for airHum
    def json_dump_airHum(self,msg):

        try:    
            with open("esp_received_msg.json", "r") as json_file_airHum_r:
                data_airHum = json.load(json_file_airHum_r)
                target_msg_airHum = data_airHum["esp32/no.1/airHum"] = msg.payload.decode("utf-8")
                print(f" <OK> [mqtt_on_message][airHum]   Opened JSON for airHum!      ")

                try:
                    with open("esp_received_msg.json", "w") as json_file_airHum_w:
                        if target_msg_airHum != None:
                            json.dump(data_airHum, json_file_airHum_w, indent = 4)
                            print(f"  <OK> [mqtt_on_message][airHum]   Dumped into JSON for airHum!  {data_airHum}    ")
                        elif target_msg_airHum == None:
                            print(f"  <!> [mqtt_on_message][airHum]   Received None Type message!  {data_airHum}    ")
                except Exception as e:
                    print(f"  <!> [mqtt_on_message][airHum]   Could not dump JSON for airHum!  {e}     ")

        except Exception as e:
                print(f" <!> [mqtt_on_message][airHum]   Could not open JSON for airHum!  {e}    ")

    # json dump for sysState
    def json_dump_sysState(self,msg):

        try:
             with open("esp_received_msg.json","r") as json_file_sysState_r:
                data_sysState = json.load(json_file_sysState_r)
                target_msg_sysState = data_sysState["esp32/no.1/sysState"] = msg.payload.decode("utf-8")


                print(f" <OK> [mqtt_on_message][sysState]   Opened JSON for sysState!      ")

                try:
                    with open("esp_received_msg.json","w") as json_file_sysState_w:
                        if target_msg_sysState != None:
                            json.dump(data_sysState, json_file_sysState_w, indent = 4)
                            print(f" <OK> [mqtt_on_message][sysState]   Dumped into JSON for sysState!  {data_sysState}    ")
                        elif target_msg_sysState == None:
                            print(f" <!> [mqtt_on_message][sysState]   Received None Type message!  {data_sysState}    ")
                except Exception as e:
                    print(f" <!> [mqtt_on_message][sysState]   Could not dump JSON for sysState!  {e}    ")
        except Exception as e:
             print(f" <!> [mqtt_on_message][sysState]   Could not open JSON for sysState!  {e}    ")

    # json dump for airPressure
    def json_dump_airPressure(self,msg):
    
        try:
                with open("esp_received_msg.json","r") as json_file_airPressure_r:
                    data_airPressure = json.load(json_file_airPressure_r)
                    target_msg_airPressure = data_airPressure["esp32/no.1/airPressure"] = msg.payload.decode("utf-8")

                    print(f" <OK> [mqtt_on_message][airPressure]   Opened JSON for airPressure!      ")

                    try:
                        with open("esp_received_msg.json","w") as json_file_airPressure_w:
                            if target_msg_airPressure != None:
                                json.dump(data_airPressure, json_file_airPressure_w, indent = 4)
                                print(f" <OK> [mqtt_on_message][airPressure]   Dumped into JSON for airPressure!  {data_airPressure}    ")
                            elif target_msg_airPressure == None:
                                print(f" <!> [mqtt_on_message][airPressure]   Received None Type message!  {data_airPressure}    ")
                    except Exception as e:
                        print(f" <!> [mqtt_on_message][airPressure]   Could not dump JSON for airPressure!  {e}    ")
        except Exception as e:
                print(f" <!> [mqtt_on_message][airPressure]   Could not open JSON for airPressure!  {e}    ")

    # json dump for airQuality
    def json_dump_IAQ(self,msg):

        try:
            with open("esp_received_msg.json","r") as json_file_IAQ_r:
                data_IAQ = json.load(json_file_IAQ_r)
                target_msg_IAQ = data_IAQ["esp32/no.1/IAQ"] = msg.payload.decode("utf-8")

                print(f" <OK> [mqtt_on_message][IAQ]   Opened JSON for IAQ!      ")

                try:
                    with open("esp_received_msg.json","w") as json_file_IAQ_w:
                        if target_msg_IAQ != None:
                            json.dump(data_IAQ, json_file_IAQ_w, indent = 4)
                            print(f" <OK> [mqtt_on_message][IAQ]   Dumped into JSON for IAQ!  {data_IAQ}    ")
                        elif target_msg_IAQ == None:
                            print(f" <!> [mqtt_on_message][IAQ]   Received None Type message!  {data_IAQ}    ")
                except Exception as e:
                    print(f" <!> [mqtt_on_message][IAQ]   Could not dump JSON for IAQ!  {e}    ")
        except Exception as e:
            print(f" <!> [mqtt_on_message][IAQ]   Could not open JSON for IAQ!  {e}    ")

    # json dump for eCO2
    def json_dump_eCO2(self,msg):

        try:
            with open("esp_received_msg.json","r") as json_file_eCO2_r:
                data_eCO2 = json.load(json_file_eCO2_r)
                target_msg_eCO2 = data_eCO2["esp32/no.1/eCO2"] = msg.payload.decode("utf-8")

                print(f" <OK> [mqtt_on_message][eCO2]   Opened JSON for eCO2!      ")

                try:
                    with open("esp_received_msg.json","w") as json_file_eCO2_w:
                        if target_msg_eCO2 != None:
                            json.dump(data_eCO2, json_file_eCO2_w, indent = 4)
                            print(f" <OK> [mqtt_on_message][eCO2]   Dumped into JSON for eCO2!  {data_eCO2}    ")
                        elif target_msg_eCO2 == None:
                            print(f" <!> [mqtt_on_message][eCO2]   Received None Type message!  {data_eCO2}    ")
                except Exception as e:
                    print(f" <!> [mqtt_on_message][eCO2]   Could not dump JSON for eCO2!  {e}    ")
        except Exception as e:
            print(f" <!> [mqtt_on_message][eCO2]   Could not open JSON for eCO2!  {e}    ")

    # json dump for TVOC
    def json_dump_TVOC(self,msg):

        try:
            with open("esp_received_msg.json","r") as json_file_TVOC_r:
                data_TVOC = json.load(json_file_TVOC_r)
                target_msg_TVOC = data_TVOC["esp32/no.1/TVOC"] = msg.payload.decode("utf-8")

                print(f" <OK> [mqtt_on_message][TVOC]   Opened JSON for TVOC!      ")

                try:
                    with open("esp_received_msg.json","w") as json_file_TVOC_w:
                        if target_msg_TVOC != None:
                            json.dump(data_TVOC, json_file_TVOC_w, indent = 4)
                            print(f" <OK> [mqtt_on_message][TVOC]   Dumped into JSON for TVOC!  {data_TVOC}    ")
                        elif target_msg_TVOC == None:
                            print(f" <!> [mqtt_on_message][TVOC]   Received None Type message!  {data_TVOC}    ")
                except Exception as e:
                    print(f" <!> [mqtt_on_message][TVOC]   Could not dump JSON for TVOC!  {e}    ")
        except Exception as e:
            print(f" <!> [mqtt_on_message][TVOC]   Could not open JSON for TVOC!  {e}    ")

    # json dump for EtOH
    def json_dump_EtOH(self,msg):

        try:
            with open("esp_received_msg.json","r") as json_file_EtOH_r:
                data_EtOH = json.load(json_file_EtOH_r)
                target_msg_EtOH = data_EtOH["esp32/no.1/EtOH"] = msg.payload.decode("utf-8")

                print(f" <OK> [mqtt_on_message][EtOH]   Opened JSON for EtOH!      ")

                try:
                    with open("esp_received_msg.json","w") as json_file_EtOH_w:
                        if target_msg_EtOH != None:
                            json.dump(data_EtOH, json_file_EtOH_w, indent = 4)
                            print(f" <OK> [mqtt_on_message][EtOH]   Dumped into JSON for EtOH!  {data_EtOH}    ")
                        elif target_msg_EtOH == None:
                            print(f" <!> [mqtt_on_message][EtOH]   Received None Type message!  {data_EtOH}    ")
                except Exception as e:
                    print(f" <!> [mqtt_on_message][EtOH]   Could not dump JSON for EtOH!  {e}    ")
        except Exception as e:
            print(f" <!> [mqtt_on_message][EtOH]   Could not open JSON for EtOH!  {e}    ")


    """
    JSON handlers for coming from DASHBOARD to RaspPI : Dashboard --> Topic --> RaspPI Python program --> JSON
    Dumps received message from topic to json file : esp_received_msg.json

    """
 
    # json dump for tempCtrl
    def json_dump_tempCtrl(self,msg):

        try:
             with open("esp_received_msg.json","r") as json_file_tempCtrl_r:
                data_tempCtrl = json.load(json_file_tempCtrl_r)
                target_msg_tempCtrl = data_tempCtrl["RaspPi/esp32/no.1/temperatureControl"] = msg.payload.decode("utf-8")


                print(f" <OK> [mqtt_on_message][tempCtrl]   Opened JSON for tempCtrl!       ")

                try:
                    with open("esp_received_msg.json","w") as json_file_tempCtrl_w:
                        if target_msg_tempCtrl != None:
                            json.dump(data_tempCtrl, json_file_tempCtrl_w, indent = 4)
                            print(f" <OK> [mqtt_on_message][tempCtrl]   Dumped into JSON for tempCtrl!  {data_tempCtrl}    ")
                        elif target_msg_tempCtrl == None:
                            print(f" <!> [mqtt_on_message][tempCtrl]   Received None Type message!  {data_tempCtrl}    ")
                except Exception as e:
                    print(f" <!> [mqtt_on_message][tempCtrl]   Could not dump JSON for tempCtrl!  {e}     ")
        except Exception as e:
             print(f" <!> [mqtt_on_message][tempCtrl]   Could not open JSON for tempCtrl!  {e}     ")

    """
    JSON handlers for coming from DASHBOARD to RaspPI : Dashboard --> Topic --> RaspPI Python program --> JSON
    Dumps received message from topic to json file : esp_received_msg.json

    """
    def json_dump_setLocation(self, msg):
            
            try:
                with open("esp_received_msg.json","r") as json_file_setLocation_r:
                    data_setLocation = json.load(json_file_setLocation_r)
                    target_msg_setLocation = data_setLocation["RaspPi/esp32/no.1/setLocation"] = msg.payload.decode("utf-8")
    
                    print(f" <OK> [mqtt_on_message][setLocation]   Opened JSON for setLocation!       ")
    
                    try:
                        with open("esp_received_msg.json","w") as json_file_setLocation_w:
                            if target_msg_setLocation != None:
                                json.dump(data_setLocation, json_file_setLocation_w, indent = 4)
                                print(f" <OK> [mqtt_on_message][setLocation]   Dumped into JSON for setLocation!  {data_setLocation}    ")
                            elif target_msg_setLocation == None:
                                print(f" <!> [mqtt_on_message][setLocation]   Received None Type message!  {data_setLocation}    ")
                    except Exception as e:
                        print(f" <!> [mqtt_on_message][setLocation]   Could not dump JSON for setLocation!  {e}     ")
            except Exception as e:
                print(f" <!> [mqtt_on_message][setLocation]   Could not open JSON for setLocation!  {e}     ")

    def json_dump_outTemp(self,target_msg_received_temperature):
        
        try:
            with open("esp_received_msg.json","r") as json_file_outTemp_r:
                data_outTemp = json.load(json_file_outTemp_r)
                data_outTemp["RaspPi/esp32/no.1/getOutsideTemperature"] = target_msg_received_temperature
                try:
                    with open("esp_received_msg.json","w") as json_file_outTemp_w:
                        if data_outTemp != None:
                            json.dump(data_outTemp, json_file_outTemp_w, indent = 4)
                            print(f" <OK> [mqtt_on_message][outTemp]   Dumped into JSON for outTemp!  {target_msg_received_temperature}    ")
                        elif data_outTemp == None:
                            print(f" <!> [mqtt_on_message][outTemp]   Received None Type message!  {target_msg_received_temperature}    ")
                except Exception as e:
                    print(f" <!> [mqtt_on_message][outTemp]   Could not dump JSON for outTemp!  {e}     ")
        except Exception as e:
             print(f" <!> [mqtt_on_message][outTemp]   Could not open JSON for outTemp!  {e}     ")


#_______________________________________________________________________________________________________________________________#
#________________________________________________________RETURN_AREA____________________________________________________________#

#_______________________________________________________________________________________________________________________________#
#________________________________________________________ DEBUG_AREA____________________________________________________________#
if __name__ == "__main__":
    received_temperature = '7'
    instance = on_message_json_dump()  # Create an instance of the class
    instance.json_dump_outTemp(received_temperature)  # Call the method on the instance