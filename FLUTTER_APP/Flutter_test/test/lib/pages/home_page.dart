// ignore_for_file: prefer_const_constructors, prefer_const_literals_to_create_immutables
import 'dart:async';
import 'package:flutter/material.dart';
import 'package:google_fonts/google_fonts.dart';
import 'package:test/room_info/room_info.dart';
import 'package:firebase_database/firebase_database.dart';

class HomePage extends StatefulWidget {
  const HomePage({super.key});

  @override
  State<HomePage> createState() => _HomePageState();
}

class _HomePageState extends State<HomePage> {
  StreamController<List<RoomInfo>> _roomInfoStreamController =
      StreamController<List<RoomInfo>>();

  Stream<List<RoomInfo>> get roomInfoStream => _roomInfoStreamController.stream;

  /*######################## INITAL LIST   ########################*/

  List<RoomInfo> roomInfos = [
    RoomInfo(
      name: 'Temperature Control',
      temperatureInside: 0,
      temperatureOutside: 0,
      temperatureCtrl: 0,
      sysState: '',
      asset: 'living_room.jpg',
    ),
    RoomInfo(
      name: 'Air pressure and humidity',
      humidity: 0,
      airPressure: 0,
      asset: 'humidity.jpg',
    ),
    RoomInfo(
      name: 'Air Quality',
      iaq: 0,
      tvoc: 0,
      etoh: 0,
      co2: 0,
      asset: 'airqaulity.jpeg',
    ),
    // Add more RoomInfo objects as needed
  ];

  /*####################################################################*/

  _HomePageState() {
    DatabaseReference ref;

    /*######################## PARSE DATA TO APP  ########################*/
    DatabaseReference refAll = FirebaseDatabase.instance.ref('NRD/ESP');
    refAll.onValue.listen((DatabaseEvent event) {
      Map<dynamic, dynamic> values = Map.from(event.snapshot.value as Map);
      print(values);

      List<RoomInfo> roomInfos = [
        RoomInfo(
          name: 'Temperature Control',
          temperatureInside: values['Temp'] != null
              ? double.parse(values['Temp'].toString())
              : 0,
          temperatureOutside: values['OutTemp'] != null
              ? double.parse(values['OutTemp'].toString())
              : 0,
          temperatureCtrl: values['TempCtrl'] != null
              ? double.parse(values['TempCtrl'].toString())
              : 0,
          sysState:
              values['SysState'] != null ? values['SysState'].toString() : '',
          asset: 'living_room.jpg',
        ),
        RoomInfo(
          name: 'Air pressure and humidity',
          humidity: values['Hum'] != null
              ? double.parse(values['Hum'].toString())
              : 0,
          airPressure: values['AirPress'] != null
              ? double.parse(values['AirPress'].toString())
              : 0,
          asset: 'humidity.jpg',
        ),
        RoomInfo(
          name: 'Air Quality',
          iaq: values['IAQ'] != null
              ? double.parse(values['IAQ'].toString())
              : 0,
          tvoc: values['TVOC'] != null
              ? double.parse(values['TVOC'].toString())
              : 0,
          etoh: values['ETOH'] != null
              ? double.parse(values['ETOH'].toString())
              : 0,
          co2: values['CO2'] != null
              ? double.parse(values['CO2'].toString())
              : 0,
          asset: 'airquality.jpeg',
        ),
        // Add more RoomInfo objects as needed
      ];

      _roomInfoStreamController.add(roomInfos);
    });

    /*####################################################################*/
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
        appBar: AppBar(
          backgroundColor: Colors.grey[900],
          iconTheme: IconThemeData(color: Colors.white),
          title: Text(
            'Smart Home by Dan C.',
            style: GoogleFonts.roboto(
              fontSize: 30,
              fontWeight: FontWeight.w700,
              fontStyle: FontStyle.normal,
              color: Color.fromARGB(255, 226, 219, 219),
              letterSpacing: 3,
            ),
          ),
        ),
        backgroundColor: Color.fromARGB(255, 0, 2, 0),
        body: SafeArea(
            child: Center(
                child: Column(
          children: [
            SizedBox(height: 20),
            Image(
              width: 350,
              height: 350,
              image: AssetImage('assets/magic_house.png'),
            ),
            SizedBox(height: 20),
            Expanded(
              child: Container(
                  padding: EdgeInsets.all(15),
                  decoration: BoxDecoration(
                      color: Color.fromARGB(255, 225, 255, 205),
                      gradient: LinearGradient(
                          begin: Alignment.topCenter,
                          end: Alignment.bottomCenter,
                          colors: [
                            Color.fromARGB(255, 100, 100, 100),
                            Color.fromARGB(255, 31, 31, 31),
                          ]),
                      borderRadius: BorderRadius.only(
                          topLeft: Radius.circular(30),
                          topRight: Radius.circular(30)),
                      boxShadow: [
                        BoxShadow(
                          color: Color.fromARGB(255, 112, 112, 112),
                          spreadRadius: 5,
                          blurRadius: 10,
                          offset: Offset(0, 2),
                        )
                      ]),
                  child: StreamBuilder<List<RoomInfo>>(
                    stream: roomInfoStream,
                    builder: (BuildContext context,
                        AsyncSnapshot<List<RoomInfo>> snapshot) {
                      if (snapshot.hasError) {
                        return Text('Error: ${snapshot.error}');
                      }
                      switch (snapshot.connectionState) {
                        case ConnectionState.waiting:
                          return const Text('Loading...');
                        default:
                          return ListView(
                            padding: EdgeInsets.all(4),
                            children: snapshot.data!.map((RoomInfo roomInfo) {
                              return RoomWidget(roomInfo: roomInfo);
                            }).toList(),
                          );
                      }
                    },
                  )),
            ),
          ],
        ))));
  }
}

class RoomWidget extends StatefulWidget {
  final RoomInfo roomInfo;

  @override
  State<RoomWidget> createState() => _RoomWidgetState();

  const RoomWidget({Key? key, required this.roomInfo}) : super(key: key);
}

class _RoomWidgetState extends State<RoomWidget> {
  final Color insideCardColor = Colors.white;
  double _currentSliderValue = 23; // Initial value for the slider
  double thermostatSlider = 23; // Initial value for the thermostat slider

  DatabaseReference writeTempValRef = FirebaseDatabase.instance.ref("NRD/ESP");

  void iterate() {
    setState(() {
      print("it works");
    });
  }

  void onChanged(double newValue) async {
      await writeTempValRef.update({
        "TempCtrl": newValue,
      });
  }

  @override
  Widget build(BuildContext context) {
    return Center(
      child: Column(children: [
        SizedBox(height: 10),
        Center(
          child: Container(
            padding: EdgeInsets.all(10),
            decoration: BoxDecoration(
                image: DecorationImage(
                  image: AssetImage(widget.roomInfo.asset),
                  fit: BoxFit.cover,
                  colorFilter: ColorFilter.mode(
                      Color.fromARGB(255, 87, 87, 87), BlendMode.multiply),
                ),
                color: Color.fromARGB(255, 192, 192, 192),
                borderRadius: BorderRadius.all(Radius.circular(20)),
                boxShadow: [
                  BoxShadow(
                    color: Color.fromARGB(255, 43, 43, 43),
                    spreadRadius: 2,
                    blurRadius: 8,
                    offset: Offset(0, 1),
                  )
                ]),
            child: InkWell(
              onTap: iterate,
              child: Column(
                crossAxisAlignment: CrossAxisAlignment.start,
                children: [
                  if (widget.roomInfo.name == 'Temperature Control') ...[
                    Row(
                      children: [
                        Expanded(
                          flex: 1,
                          child: Icon(
                            Icons.house_outlined,
                            size: 50,
                            color: insideCardColor,
                          ),
                        ),
                        Expanded(
                            flex: 8,
                            child: Text('Inside temperture:',
                                style: GoogleFonts.roboto(
                                  fontSize: 20,
                                  fontWeight: FontWeight.w700,
                                  fontStyle: FontStyle.normal,
                                  color: insideCardColor,
                                ))),
                        Expanded(
                            child:
                                Text('${widget.roomInfo.temperatureInside} °C',
                                    style: GoogleFonts.roboto(
                                      fontSize: 20,
                                      fontWeight: FontWeight.w700,
                                      fontStyle: FontStyle.normal,
                                      color: insideCardColor,
                                    ))),
                      ],
                    ),
                    Row(children: [
                      Expanded(
                        flex: 1,
                        child: Icon(
                          widget.roomInfo.temperatureOutside < 5
                              ? Icons.ac_unit
                              : widget.roomInfo.temperatureOutside > 30
                                  ? Icons.outdoor_grill_rounded
                                  : Icons.sunny,
                          size: 50,
                          color: insideCardColor,
                        ),
                      ),
                      Expanded(
                          flex: 8,
                          child: Text('Outside temperture:',
                              style: GoogleFonts.roboto(
                                fontSize: 20,
                                fontWeight: FontWeight.w700,
                                fontStyle: FontStyle.normal,
                                color: insideCardColor,
                              ))),
                      Expanded(
                          child:
                              Text('${widget.roomInfo.temperatureOutside} °C',
                                  style: GoogleFonts.roboto(
                                    fontSize: 20,
                                    fontWeight: FontWeight.w700,
                                    fontStyle: FontStyle.normal,
                                    color: insideCardColor,
                                  ))),
                    ]),
                    Row(children: [
                      Expanded(
                        flex: 1,
                        child: Icon(
                          widget.roomInfo.sysState == "COOLING"
                              ? Icons.ac_unit_sharp
                              : widget.roomInfo.sysState == "HEATING"
                                  ? Icons.fireplace
                                  : Icons.power_off,
                          size: 50,
                          color: insideCardColor,
                        ),
                      ),
                      Expanded(
                          flex: 8,
                          child: Text('Climate control state:',
                              style: GoogleFonts.roboto(
                                fontSize: 20,
                                fontWeight: FontWeight.w700,
                                fontStyle: FontStyle.normal,
                                color: insideCardColor,
                              ))),
                      Expanded(
                          child: Text('${widget.roomInfo.sysState} ',
                              style: GoogleFonts.roboto(
                                fontSize: 20,
                                fontWeight: FontWeight.w700,
                                fontStyle: FontStyle.normal,
                                color: insideCardColor,
                              ))),
                    ]),
                    Row(children: [
                      Expanded(
                        flex: 1,
                        child: Icon(
                          Icons.thermostat,
                          size: 50,
                          color: insideCardColor,
                        ),
                      ),
                      Expanded(
                        flex: 8,
                        child: Text('Thermostat setting:',
                            style: GoogleFonts.roboto(
                              fontSize: 20,
                              fontWeight: FontWeight.w700,
                              fontStyle: FontStyle.normal,
                              color: insideCardColor,
                            ))),
                      Expanded(
                          child: Text('${widget.roomInfo.temperatureCtrl} °C',
                              style: GoogleFonts.roboto(
                                fontSize: 20,
                                fontWeight: FontWeight.w700,
                                fontStyle: FontStyle.normal,
                                color: insideCardColor,
                              ))),
                      Slider(
                        value: thermostatSlider,
                        min: 15,
                        max: 31,
                        divisions: (31 - 15) * 2,
                        onChanged: (double newValue) {
                          setState(() {
                            thermostatSlider = newValue;
                            widget.roomInfo.temperatureCtrl =
                                newValue; // Update the thermostat setting
                                onChanged(newValue);
                          });
                        },
                      )
                    ]),
                  ],
                  if (widget.roomInfo.name == 'Air Quality') ...[
                    Row(
                      children: [
                        Expanded(
                          flex: 1,
                          child: Icon(
                            Icons.air,
                            size: 50,
                            color: insideCardColor,
                          ),
                        ),
                        Expanded(
                            flex: 8,
                            child: Text('IAQ',
                                style: GoogleFonts.roboto(
                                  fontSize: 20,
                                  fontWeight: FontWeight.w700,
                                  fontStyle: FontStyle.normal,
                                  color: insideCardColor,
                                ))),
                        Expanded(
                            child: Text('${widget.roomInfo.iaq}',
                                style: GoogleFonts.roboto(
                                  fontSize: 20,
                                  fontWeight: FontWeight.w700,
                                  fontStyle: FontStyle.normal,
                                  color: insideCardColor,
                                ))),
                      ],
                    ),
                    Row(
                      children: [
                        Expanded(
                          flex: 1,
                          child: Icon(
                            Icons.co2_rounded,
                            size: 50,
                            color: insideCardColor,
                          ),
                        ),
                        Expanded(
                            flex: 8,
                            child: Text('CO2',
                                style: GoogleFonts.roboto(
                                  fontSize: 20,
                                  fontWeight: FontWeight.w700,
                                  fontStyle: FontStyle.normal,
                                  color: insideCardColor,
                                ))),
                        Expanded(
                            child: Text('${widget.roomInfo.co2} ppm',
                                style: GoogleFonts.roboto(
                                  fontSize: 20,
                                  fontWeight: FontWeight.w700,
                                  fontStyle: FontStyle.normal,
                                  color: insideCardColor,
                                ))),
                      ],
                    ),
                    Row(
                      children: [
                        Expanded(
                          flex: 1,
                          child: Icon(
                            Icons.cloud_off,
                            size: 50,
                            color: insideCardColor,
                          ),
                        ),
                        Expanded(
                            flex: 8,
                            child: Text('TVOC',
                                style: GoogleFonts.roboto(
                                  fontSize: 20,
                                  fontWeight: FontWeight.w700,
                                  fontStyle: FontStyle.normal,
                                  color: insideCardColor,
                                ))),
                        Expanded(
                            child: Text('${widget.roomInfo.tvoc} ppm',
                                style: GoogleFonts.roboto(
                                  fontSize: 20,
                                  fontWeight: FontWeight.w700,
                                  fontStyle: FontStyle.normal,
                                  color: insideCardColor,
                                ))),
                      ],
                    ),
                    Row(
                      children: [
                        Expanded(
                          flex: 1,
                          child: Icon(
                            Icons.cloud_queue,
                            size: 50,
                            color: insideCardColor,
                          ),
                        ),
                        Expanded(
                            flex: 8,
                            child: Text('ETOH',
                                style: GoogleFonts.roboto(
                                  fontSize: 20,
                                  fontWeight: FontWeight.w700,
                                  fontStyle: FontStyle.normal,
                                  color: insideCardColor,
                                ))),
                        Expanded(
                            child: Text('${widget.roomInfo.etoh} ppm',
                                style: GoogleFonts.roboto(
                                  fontSize: 20,
                                  fontWeight: FontWeight.w700,
                                  fontStyle: FontStyle.normal,
                                  color: insideCardColor,
                                ))),
                      ],
                    ),
                  ],
                  if (widget.roomInfo.name == 'Air pressure and humidity') ...[
                    Row(
                      children: [
                        Expanded(
                          flex: 1,
                          child: Icon(
                            Icons.speed_outlined,
                            size: 50,
                            color: insideCardColor,
                          ),
                        ),
                        Expanded(
                            flex: 8,
                            child: Text('Air pressure',
                                style: GoogleFonts.roboto(
                                  fontSize: 20,
                                  fontWeight: FontWeight.w700,
                                  fontStyle: FontStyle.normal,
                                  color: insideCardColor,
                                ))),
                        Expanded(
                            child: Text('${widget.roomInfo.airPressure} hPa',
                                style: GoogleFonts.roboto(
                                  fontSize: 20,
                                  fontWeight: FontWeight.w700,
                                  fontStyle: FontStyle.normal,
                                  color: insideCardColor,
                                ))),
                      ],
                    ),
                    Row(
                      children: [
                        Expanded(
                          flex: 1,
                          child: Icon(
                            Icons.water_drop_outlined,
                            size: 50,
                            color: insideCardColor,
                          ),
                        ),
                        Expanded(
                            flex: 8,
                            child: Text('Air humidity',
                                style: GoogleFonts.roboto(
                                  fontSize: 20,
                                  fontWeight: FontWeight.w700,
                                  fontStyle: FontStyle.normal,
                                  color: insideCardColor,
                                ))),
                        Expanded(
                            child: Text('${widget.roomInfo.humidity}%',
                                style: GoogleFonts.roboto(
                                  fontSize: 20,
                                  fontWeight: FontWeight.w700,
                                  fontStyle: FontStyle.normal,
                                  color: insideCardColor,
                                ))),
                      ],
                    ),
                  ],
                ],
              ),
            ),
          ),
        ),
      ]),
    );
  }
}
