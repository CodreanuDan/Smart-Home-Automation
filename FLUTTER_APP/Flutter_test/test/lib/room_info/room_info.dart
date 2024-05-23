// class RoomInfo {
//   double temperatureInside;
//   double humidity;
//   double temperatureOutside;

//   String asset;

//   RoomInfo(
//       {this.temperatureInside = 0,
//       this.humidity = 0,
//       this.temperatureOutside = 0,
//       this.asset = ''});
// }

class RoomInfo {
  String name;
  double temperatureInside;
  double temperatureOutside;
  double temperatureCtrl;
  String sysState;
  double humidity;
  double airPressure;
  double iaq;
  double tvoc;
  double etoh;
  double co2;
  String asset;

  RoomInfo({
    this.name = '',
    this.temperatureInside = 0,
    this.temperatureOutside = 0,
    this.temperatureCtrl = 0,
    this.sysState = '',
    this.humidity = 0,
    this.airPressure = 0,
    this.iaq = 0,
    this.tvoc = 0,
    this.etoh = 0,
    this.co2 = 0,
    this.asset = '',
  });
}
