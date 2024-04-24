#_______________________________________________________________________________________________________________________________#
#_______________________________________________________DESCRIPTION:____________________________________________________________#


#_______________________________________________________________________________________________________________________________#

#_______________________________________________________________________________________________________________________________#
#_____________________________________________________IMPORT_LIBRARIES__________________________________________________________#
import python_weather as py_wth
import asyncio
import aiohttp
import time


#_______________________________________________________________________________________________________________________________#
#______________________________________________________IMPORT MODULES___________________________________________________________#

#_______________________________________________________________________________________________________________________________#
#_____________________________________________________GLOBAL_VARIABLES__________________________________________________________#
#_______________________________________________________________________________________________________________________________#
#_______________________________________________________MAIN_FUNCTION___________________________________________________________#
class get_outside_temp():

    try:
        async def get_weather(location):
            if location == None or location == " " or location == "":
                print(f" <!> [get_outside_temp]   Location is empty.     ")
                return None
            else:
                for _ in range(3):       # Retry 3 times "_"; stands for a throwaway variable
                    try:
                        async with py_wth.Client(unit=py_wth.METRIC) as client:
                            weather = await client.get(location)
                            print(f"[get_outside_temp] Temperature: {weather.temperature}°C")
                            # print(type(weather.temperature)) #for debugging purposes
                            received_temperature = weather.temperature
                            return str(received_temperature)
                    except aiohttp.client_exceptions.ServerDisconnectedError:
                        print("Server disconnected, retrying...")
                        await asyncio.sleep(5)  # Wait for 5 seconds before retrying
                    except Exception as e:
                        print(f" <!> [get_outside_temp]   Could not get the temperature. {e}     ")
                        break  # If the error is not ServerDisconnectedError, break the loop
    except Exception as e:
        print(f" <!> [get_outside_temp]   Could not get the temperature. {e}     ")
        
#_______________________________________________________________________________________________________________________________#
#________________________________________________________RETURN_AREA____________________________________________________________#

#_______________________________________________________________________________________________________________________________#
#________________________________________________________ DEBUG_AREA____________________________________________________________#
if __name__ == "__main__":
    locations = [None,"  "," "," ",None,"Barcelona", "Iasi", "Bucharest", "New York",000,"ygdygfgfyrfr","Tokyo", "Sydney", "Cape Town"]	
    for location in locations:
        print(asyncio.run(get_outside_temp.get_weather(location   = location)))
        print(location)
        time.sleep(0.25)
